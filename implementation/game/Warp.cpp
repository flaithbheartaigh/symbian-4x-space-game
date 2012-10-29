// Symbian 4X Space Game
// 
// Copyright (C) 2011 Patrick Pelletier
// 
// This program is free software: you can redistribute it and/or modify it
// under the  terms of the GNU  General Public License as published by the
// Free Software Foundation, either version 3 of the License, or any later
// version.
// 
// This  program  is distributed  in  the  hope  that  it will  be  useful,
// but  WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
// Public License for more details.
// 
// You should have received a copy  of the GNU General Public License along
// with this program. See <http://www.opensource.org/licenses/gpl-3.0.html>

#include "Warp.h"
#include "Sector.h"
#include "StarSystem.h"
#include "UniverseVisitor.h"
#include "Universe.h"
#include "dijkstra.h"

using namespace Game;

std::vector<SectorReference> Warp::path(Sector * from, Sector * to)
{
    std::vector<SectorReference> pathReturn;

    if (from != to)
    {
        std::vector<Sector *> vertices;
        std::map<Sector *, int> indexes;
        std::map<std::pair<Sector *, Sector *>, double> edges;

        // Edges in starting system
        indexes[from] = vertices.size();
        vertices.push_back(from);
        if (from->starSystem() == to->starSystem())
        {
            edges[std::pair<Sector *, Sector *>(from, to)] = Sector::distance(from, to);
        }
        std::set<Warp *> warps = from->starSystem()->warps();
        for (std::set<Warp *>::const_iterator it = warps.begin(); it != warps.end(); ++it)
        {
            if (from != (*it)->sector())
            {
                edges[std::pair<Sector *, Sector *>(from, (*it)->sector())] = Sector::distance(from, (*it)->sector());
                //edges[std::pair<Sector *, Sector *>(warps[i]->sector(), from)] = Sector::distance(from, warps[i]->sector());
            }
        }
        for (unsigned int i = 0; i < Universe::instance().starSystems().size(); ++i)
        {
            warps = Universe::instance().starSystems()[i]->warps();
            for (std::set<Warp *>::const_iterator it = warps.begin(); it != warps.end(); ++it)
            {
                indexes[(*it)->sector()] = vertices.size();
                vertices.push_back((*it)->sector());
                // Warp to Warp between connected systems (warps)
                if ((*it)->destination().isValid())
                {
                    edges[std::pair<Sector *, Sector *>((*it)->sector(), (*it)->destination().sector())] = 1;
                    //edges[std::pair<Sector *, Sector *>((*it)->destination().sector(), (*it)->sector())] = 1;
                }
                // Warp to Warp inside systems (moves)
                for (std::set<Warp *>::const_iterator it2 = warps.begin(); it2 != warps.end(); ++it2)
                {
                    if (*it != *it2)
                    {
                        edges[std::pair<Sector *, Sector *>((*it)->sector(), (*it2)->sector())] = Sector::distance((*it)->sector(), (*it2)->sector());
                        //edges[std::pair<Sector *, Sector *>((*it2)->sector(), (*it)->sector())] = Sector::distance((*it)->sector(), (*it2)->sector());
                    }
                }
            }
        }
        // Edges in ending system
        indexes[to] = vertices.size();
        vertices.push_back(to);
        warps = to->starSystem()->warps();
        for (std::set<Warp *>::const_iterator it = warps.begin(); it != warps.end(); ++it)
        {
            if (to != (*it)->sector())
            {
                edges[std::pair<Sector *, Sector *>(to, (*it)->sector())] = Sector::distance(to, (*it)->sector());
                //edges[std::pair<Sector *, Sector *>(warps[i]->sector(), to)] = Sector::distance(to, warps[i]->sector());
            }
        }

        adjacency_map_t adjacency_map;
        for (std::map<std::pair<Sector *, Sector *>, double>::const_iterator it = edges.begin(); it != edges.end(); ++it)
        {
            std::pair<Sector *, Sector *> sectorPair = it->first;
            adjacency_map[indexes[sectorPair.first]].push_back(edge(indexes[sectorPair.second], it->second));
            adjacency_map[indexes[sectorPair.second]].push_back(edge(indexes[sectorPair.first], it->second));
        }
        std::map<vertex_t, weight_t> min_distance;
        std::map<vertex_t, vertex_t> previous;
        DijkstraComputePaths(0, adjacency_map, min_distance, previous);
        std::list<vertex_t> path = DijkstraGetShortestPathTo(indexes[to], previous);

        for (std::list<vertex_t>::reverse_iterator it = path.rbegin(); it != path.rend(); ++it)
        {
            pathReturn.push_back(SectorReference(vertices[*it]));
        }
        pathReturn.pop_back();
    }
    return pathReturn;
}

bool Warp::connect(Sector * from, Sector * to)
{
    if (from != NULL && to != NULL && from->isEmpty() && to->isEmpty())
    {
        Warp * a = new Warp(from);
        a->setDestination(SectorReference(to));
        Warp * b = new Warp(to);
        b->setDestination(SectorReference(from));
        from->setWarp(a);
        to->setWarp(b);
        return true;
    }
    return false;
}

Warp::~Warp()
{
    
}

Warp::Warp()
    : mSector(NULL)
    , mDestination()
{

}

Warp::Warp(Sector * sector)
    : mSector(sector)
    , mDestination()
{

}

Sector * Warp::sector() const
{
    return mSector;
}

void Warp::setSector(Sector * sector)
{
    mSector = sector;
}

const SectorReference & Warp::destination() const
{
    return mDestination;
}

void Warp::setDestination(const SectorReference & destination)
{
    mDestination = destination;
}

void Warp::accept(UniverseVisitor * visitor)
{
    if (visitor != NULL)
    {
        visitor->visit(this);
    }
}