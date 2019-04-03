#include <string>
#include <vector>
#include <stack>
#include <queue>

#include "Mesh/mesh.h"

using namespace std;

#define ADD_PROPERTY(T, x) \
private:\
    T m_##x; \
public:\
    inline T & x() { return m_##x; } \

class CBVertex
{
};
class CBEdge
{
};
class CBFace
{
    ADD_PROPERTY(int, index)
    ADD_PROPERTY(bool, visited)
};
class CBHalfEdge
{
	ADD_PROPERTY(bool, looped)
};

using CMesh = MeshLib::CBaseMesh<CBVertex, CBEdge, CBFace>;
// CMesh::CVertex is inherited from CBVertex
using CVertex = typename CMesh::CVertex;
using CEdge = typename CMesh::CEdge;
using CFace = typename CMesh::CFace;
using CHalfEdge = typename CMesh::CHalfEdge;
using CPoint = MeshLib::CPoint;
using CPoint2 = MeshLib::CPoint2;

int main()
{
    CMesh * mesh = new CMesh();
    mesh->read_m("data/eight.m");
    mesh->read_txt("data/cat.txt");
    
    for (CFace *face : mesh->faces())
    {
        face->visited() = false;
    }
    // traverse mesh faces using bfs
    CFace * root_face = mesh->face(1);
    std::queue<CFace*> queue;
    queue.push(root_face);
    int k = 0;
    while(!queue.empty())
    {
        CFace * face = queue.front();
        queue.pop();
        if(face->visited())
        {
            continue;
        }
        face->visited() = true;
        face->index() = k++;
        
        for(CHalfEdge *he : face->halfedges())
        {
            CHalfEdge *he_dual = he->dual();
            if (he_dual && !he_dual->face()->visited())
            {
                queue.push(he_dual->face());
            }
        }
    }

    return 0;
}
