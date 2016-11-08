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
    std::tr1::unordered_map<Dcel::Vertex*, Dcel::Vertex*> mapOfVertices;

    //For Each Face in Visible Faces
    for(auto faceIterator = facesVisibleByVertex->begin(); faceIterator != facesVisibleByVertex->end(); faceIterator++){
        Dcel::HalfEdge* outerHalfEdge     = (*faceIterator)->getOuterHalfEdge();
        Dcel::HalfEdge* outerHalfEdgeTwin = outerHalfEdge->getTwin();

        //If outerHalfEdgeTwin
        if( outerHalfEdgeTwin != nullptr ){
           //Loop through face's halfedges
           for( int i = 0; i<3; i++ ){
              //Get its face
              Dcel::Face* twinsFace = outerHalfEdgeTwin->getFace();
              //If the face is not visible by the Vertex
              if(facesVisibleByVertex->count(twinsFace) == 0){
                 //Add current halfedge from and to vertex to a map
                 mapOfVertices[outerHalfEdgeTwin->getFromVertex()] = outerHalfEdgeTwin->getToVertex();
                 //Add current twin to unordered horizon
                 horizon.push_back(outerHalfEdgeTwin);
               }
                 //Get Next HalfEdge
                 outerHalfEdge     = (*outerHalfEdge).getNext();
                 outerHalfEdgeTwin = (*outerHalfEdge).getTwin();
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
std::vector<Dcel::HalfEdge*> ConvexHullBuilderHelper::orderHorizon(std::vector<Dcel::HalfEdge*> unHorizon, std::tr1::unordered_map<Dcel::Vertex*, Dcel::Vertex*> &map){
  //Initialize Ordered Horizon
  std::vector<Dcel::HalfEdge*> orderedHorizon;
  //Initialize unordered horizon size
  unsigned int unHorizonSize = unHorizon.size();
  //Initialize from and to Vertex
  Dcel::Vertex* fromVertex = new Dcel::Vertex;
  Dcel::Vertex* toVertex   = new Dcel::Vertex;

    //While all the halfedges haven't been ordered
  while(unHorizonSize > orderedHorizon.size()){
    //The very first time
    if( orderedHorizon.empty() ){
      //Take from vertex of first's halfedge in the horizon
      fromVertex = unHorizon.front()->getFromVertex();
    } else {
      //The next fromVertex is the old toVertex
      fromVertex = toVertex;
    }
    //if fromVertex exists
    if( fromVertex != nullptr){
      //Get to vertex using the map. For [fromVertex] will contain only the next toVertex
      toVertex = map[fromVertex];

      //For All the Halfedges
      for(auto unHorizonIterator = unHorizon.begin(); unHorizonIterator != unHorizon.end(); unHorizonIterator ++ ){
         //Get Current HalfEdge
         Dcel::HalfEdge* currHalfEdge = *unHorizonIterator;
         //If that halfedge has exact same from and to vertex of the ordered ones
         if( currHalfEdge->getFromVertex() == fromVertex && currHalfEdge->getToVertex() == toVertex ){
             //Add it to the ordered horizon
             orderedHorizon.push_back(currHalfEdge);
         }
       }
    }

  }

  //Return ordered horizon
  return orderedHorizon;
}
