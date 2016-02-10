/** Copyright (C) 2015 Ultimaker - Released under terms of the AGPLv3 License */
#ifndef UTILS_BUCKET_GRID_2D_H
#define UTILS_BUCKET_GRID_2D_H

#include <unordered_map>
#include <vector>

#include "logoutput.h"
#include "intpoint.h"

namespace cura
{

/*!
 * Container for items with location for which the lookup for nearby items is optimized.
 * 
 * It functions by hashing the items location and lookuping up based on the hash of that location and the hashes of nearby locations.
 * 
 * Instead of mapping a cell location multiple times to an object within the cell, 
 * each cell location is mapped only once to a vector of objects within the cell.
 * 
 * The first alternative has the overhead of 'bucket-collisions' where all mappings of two different cells get placed in the same bucket, 
 * which causes findNearby to loop over unneeded elements.
 * The current implementation has the overhead and indirection of creating vectors and all that comes with it."

 */
template<typename T>
class BucketGrid2D
{
private:

    typedef Point CellIdx;
    
    /*!
     * Returns a point for which the hash is at a grid position of \p relativeHash relative to \p p.
     * 
     * \param p The point for which to get the relative point to hash
     * \param relativeHash The relative position - in grid terms - of the relative point. 
     * \return A point for which the hash is at a grid position of \p relativeHash relative to \p p.
     */
    inline Point getRelativeForHash(const Point& p, const Point& relativeHash)
    {
        return p + relativeHash*squareSize;
    }


    /*!
     * A hash class representing the hash function object.
     */
    struct CellIdxHasher
    {
        
        /*!
         * The hash function for a 2D position.
         * \param point The location to hash
         * \return the hash
         */
        inline uint32_t pointHash(const CellIdx& point) const
        {
            return point.X ^ (point.Y << 8);
        }
        
        /*!
         * See PointHasher::pointHash
         */
        uint32_t operator()(const CellIdx& p) const { return pointHash(p); };

    };
    
    
    struct Elem 
    {
        Point pos;
        T object;
        Elem(Point pos, T object)
        : pos(pos)
        , object(object)
        {
        }
    };
    
    typedef std::vector<Elem> Neighborhood;
    /*!
     * The map type used to associate points with their objects.
     */
    typedef typename std::unordered_map<CellIdx, Neighborhood, CellIdxHasher> Map;
    
    /*!
     * A helper predicate object which allways returns false when comparing two objects.
     * 
     * This is used for mapping each point to a unique object, even when two objects have the same point associated with it.
     */
    struct NeverEqual
    {
        template<typename S>
        bool operator()(const S& p1, const S& p2) const { return false; };
    };
private:
    int squareSize; //!< The horizontal and vertical size of a cell in the grid; the width and height of a bucket.
    /*!
     * The map used to associate points with their objects.
     */
    Map point2neighborhood;

    /*!
     * TODO
     */
    CellIdx getCellIdx(const Point& point) const
    {
        return point / squareSize;
    }
public:
    /*!
     * The constructor for a bucket grid.
     * 
     * \param squareSize The horizontal and vertical size of a cell in the grid; the width and height of a bucket.
     * \param initial_map_size The number of elements to be inserted 
     */
    BucketGrid2D(int squareSize, unsigned int initial_map_size = 4)
    : squareSize(squareSize)
    , point2neighborhood(initial_map_size, CellIdxHasher())
    {
        point2neighborhood.reserve(initial_map_size);
    }
    /*!
     * Insert a new point into the bucket grid.
     * 
     * \param p The location associated with \p t.
     * \param t The object to insert in the grid cell for position \p p.
     */
    void insert(Point& p, T& t)
    {
        typedef typename Map::iterator iter;
        Neighborhood new_neighborhood;
        new_neighborhood.emplace_back(p, t);
        std::pair<iter, bool> emplaced = point2neighborhood.emplace(getCellIdx(p), new_neighborhood); // try to create a new Neighborhood
        if (!emplaced.second)
        {
            emplaced.first->second.emplace_back(p, t); // insert it in the Neighborhood  
        }
    };

    /*!
     * Find all objects with a point in a grid cell at a distance of one cell from the cell of \p p.
     * 
     * \warning Objects may occur multiple times in the output!
     * 
     * \param p The point for which to find close points.
     * \param ret Ouput parameter: all objects close to \p p.
     */
    void findNearbyObjects(Point& p, std::vector<T>& ret)
    {
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                typename Map::const_iterator found = point2neighborhood.find(getCellIdx(p) + CellIdx(x,y));
                if (found == point2neighborhood.end())
                {
                    continue;
                }
                Neighborhood points_in_cell = found->second;
                for (Elem& elem : points_in_cell)
                {
                    ret.push_back(elem.object);
                }
            }
        }
    };
    
    /*!
     * Find all objects with a point in a grid cell at a distance of one cell from the cell of \p p.
     * 
     * \warning Objects may occur multiple times in the output!
     * 
     * \param p The point for which to find close points.
     * \return All objects close to \p p.
     */
    std::vector<T> findNearbyObjects(Point& p)
    {
        std::vector<T> ret;
        findNearbyObjects(p, ret);
        return ret;
    }

    /*!
     * Find the nearest object to a given lcoation \p p, if there is any in a neighboring cell in the grid.
     * 
     * \param p The point for which to find the nearest object.
     * \param nearby Output parameter: the nearest object, if any
     * \return Whether an object has been found.
     */
    bool findNearestObject(Point& p, T& nearby)
    {
        bool found = false;
        int64_t bestDist2 = squareSize*9; // 9 > sqrt(2*2 + 2*2)^2  which is the square of the largest distance of a point to a point in a neighboring cell
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                typename Map::const_iterator found = point2neighborhood.find(getCellIdx(p) + CellIdx(x,y));
                if (found == point2neighborhood.end())
                {
                    continue;
                }
                Neighborhood points_in_cell = found.second;
                for (Elem& elem : points_in_cell)
                {
                    int32_t dist2 = vSize2(elem.p - p);
                    if (dist2 < bestDist2)
                    {
                        found = true;
                        nearby = elem.t;
                        bestDist2 = dist2;
                    }
                }
            }
        }
        return found;
    };






};

}//namespace cura
#endif//BUCKET_GRID_2D_H
