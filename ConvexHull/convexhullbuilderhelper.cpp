#include "convexhullbuilderhelper.h"

/** Class which contains all the methods used by ConvexHullBuilder **/
ConvexHullBuilderHelper::ConvexHullBuilderHelper(DrawableDcel *dcel){
    this->dcel = dcel;
}

/** Class Destructor **/
ConvexHullBuilderHelper::~ConvexHullBuilderHelper(){}

/**
 * @brief  std::vector<Dcel::Vertex*> ConvexHullBuilderHelper::getAllVertices
 *         Takes all vertices from the Dcel and saves them into an arra of pointers to vertices
 * @return returns array of pointers to all vertices
 */
std::vector<Dcel::Vertex*> ConvexHullBuilderHelper::getAllVertices(){
    //Initialize vertices array
    std::vector<Dcel::Vertex*> allVertices;
    //Add all vertices to the latter
    for (auto vertex = dcel->vertexBegin(); vertex != dcel->vertexEnd(); vertex++){
        Dcel::Vertex* currentvertex = new Dcel::Vertex((*vertex)->getCoordinate());
        allVertices.push_back(currentvertex);
    }
    //Return array of pointers to vertices
    return allVertices;
}

/**
 * @brief  std::vector<Dcel::HalfEdge*> ConvexHullBuilder::bringMeTheHorizon
 *         Takes current set of visible faces and returns a vector of ordered halfedges which form the
 *         horizon.
 *         - Loop through faces
 *           - For each HalfEdge in the face
 *             - If current given halfedge's twin is not null
 *               - Check if twin's face is visible by the current vertex
 *               - If it is not visible then the twin belongs to the horizon
 *               - Add it to the horizon and set its to and from vertex in a map
 *         - Order the horizon using the map and the horizon itself
 *
 * @param  std::tr1::unordered_set<Dcel::Face*>facesVisibleByVertex
 * @return returns array of pointers to halfedges belonging to the horizon
 */
std::vector<Dcel::HalfEdge*> ConvexHullBuilderHelper::bringMeTheHorizon(std::tr1::unordered_set<Dcel::Face*>* facesVisibleByVertex){

    //Initialize Horizon
    std::vector<Dcel::HalfEdge*> horizon;

    //Initialize a map which maps each important from vertex to its to vertex (only on horizon edges)
    std::tr1::unordered_map<Dcel::Vertex*, Dcel::Vertex*> *mapOfVertices = new std::tr1::unordered_map<Dcel::Vertex*, Dcel::Vertex*>;

    //For Each Face in Visible Faces
    for(auto faceIterator = facesVisibleByVertex->begin(); faceIterator != facesVisibleByVertex->end(); faceIterator++){

        Dcel::Face* currFace = *faceIterator;

        Dcel::HalfEdge* outerHalfEdge = new Dcel::HalfEdge;
        outerHalfEdge = currFace->getOuterHalfEdge();
        Dcel::HalfEdge* outerHalfEdgeTwin = new Dcel::HalfEdge;

        //Loop through face's halfedges
        for( auto halfEdgeIterator = currFace->incidentHalfEdgeBegin(); halfEdgeIterator != currFace->incidentHalfEdgeEnd(); halfEdgeIterator++ ){

            Dcel::HalfEdge* currHalfEdge = *halfEdgeIterator;

            if( currHalfEdge->getTwin() != nullptr ){
                outerHalfEdgeTwin = currHalfEdge->getTwin();

                Dcel::Face* twinsFace = new Dcel::Face;

                //Get its face
                twinsFace = outerHalfEdgeTwin->getFace();
                //If the face is not visible by the Vertex
                if( facesVisibleByVertex->count(twinsFace) == 0 ){
                    Dcel::Vertex* outerHalfEdgeFrom     = outerHalfEdgeTwin->getFromVertex();
                    Dcel::Vertex* outerHalfEdgeTwinFrom = outerHalfEdgeTwin->getToVertex();
                    //Add current halfedge from and to vertex to a map
                    (*mapOfVertices)[outerHalfEdgeFrom] = outerHalfEdgeTwinFrom;
                    //Add current twin to unordered horizon
                    horizon.push_back(outerHalfEdgeTwin);
                }

            }

        }

    }

    //Get the ordered Horizon
    horizon = orderHorizon(horizon, mapOfVertices);

    //Return populated Horizon
    return horizon;
}

/**
 * @brief std::vector<Dcel::HalfEdge*> ConvexHullBuilder::orderHorizon
 *        Reorders the Horizon for future utilizations (setting twins) using from and to vertex from the map
 * @param std::vector<Dcel::HalfEdge*> unHorizon, std::tr1::unordered_map<Dcel::Vertex*, Dcel::Vertex*>
 * @return ordered Horizon
 */
std::vector<Dcel::HalfEdge*> ConvexHullBuilderHelper::orderHorizon(std::vector<Dcel::HalfEdge*> unHorizon, std::tr1::unordered_map<Dcel::Vertex*, Dcel::Vertex*> *map){
    //Initialize Ordered Horizon
    std::vector<Dcel::HalfEdge*> orderedHorizon;

    //Initialize from and to Vertex
    Dcel::Vertex* fromVertex = new Dcel::Vertex;
    Dcel::Vertex* toVertex   = new Dcel::Vertex;

    //While all the halfedges haven't been ordered
    while(unHorizon.size() != orderedHorizon.size()){
        //The very first time
        if( orderedHorizon.size() == 0 ){
            //Take from vertex of first's halfedge in the horizon
            fromVertex = unHorizon[0]->getFromVertex();
        } else {
            //The next fromVertex is the old toVertex
            fromVertex = toVertex;
        }

        //if fromVertex exists
        if( fromVertex != nullptr){
            //Get to vertex using the map. For [fromVertex] will contain only the next toVertex
            toVertex = (*map)[fromVertex];

            //For All the Halfedges
            for(auto unHorizonIterator = unHorizon.begin(); unHorizonIterator != unHorizon.end(); unHorizonIterator ++ ){
                //If that halfedge has exact same from and to vertex of the ordered ones
                if( (*unHorizonIterator)->getFromVertex() == fromVertex && (*unHorizonIterator)->getToVertex() == toVertex ){
                    //Add it to the ordered horizon
                    orderedHorizon.push_back(*unHorizonIterator);
                }
            }
        }

    }

    //Return ordered horizon
    return orderedHorizon;
}
