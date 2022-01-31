#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <vector>

template <class GRAPH> class BFS;


template <class T>
class graph
{

public:

    typedef T Vertex;
    typedef std::unordered_multiset<T> VertexSet;
    typedef std::vector<T> Path;

    graph<T>()
    {

    }

    std::size_t n() const
    {
        return _t.size();  // number of vertices is size of _t
    }

    std::size_t m() const
    {
        std::size_t ans(0);

        for (auto p: _t)
            ans += p.second.size();

        return ans/2;
    }

    VertexSet V() const
    {
        VertexSet ans;

        for (auto p: _t)
            ans.insert(p.first);

        return ans;
    }

    bool isVertex(const T & v) const
    {
        return (_t.count(v) != 0);
    }

    bool isEdge(const T & v, const T & w) const
    {
        return (isVertex(v) && isVertex(w) && _t.at(v).count(w) > 0);
    }

    VertexSet Adj(const T & v) const
    {
        assert(isVertex(v));
        return _t.at(v);
    }

    std::size_t deg(const T & v) const
    {
        assert(isVertex(v));
        std::size_t count = 0;
        for (auto &w: Adj(v)) { 
            if (w == v) {
                count += 2;
            }
            else {
                ++count;
            }
        }
        return count;
    }

    void addVertex(const T & v)
    {
        if (isVertex(v))
            return;

        _t[v] = VertexSet();

    }

    void addEdge(const T & v, const T & w)
    {
        assert(isVertex(v) && isVertex(w));
        if (v != w) {
            _t[v].insert(w);
        }
        _t[w].insert(v);
    }

    void removeEdge(const T & v, const T & w)
    {
        if (!isEdge(v, w))
            return;
        if (v != w) {
            _t[v].erase(_t.at(v).find(w));
        }
        _t[w].erase(_t.at(w).find(v));
    }

    bool isConnected() const
    {
        BFS<graph> B(*this);
        return (B.ncc() == 1);

    }

    std::size_t ncc() const
    {
        BFS<graph> B(*this);
        return B.ncc();
    }

    bool isAcyclic() const
    {
        return (n() == ncc() + m());
    }

    bool isTree() const
    {
        return (isConnected() && isAcyclic());
    }




private:

    std::unordered_map<Vertex, VertexSet> _t;

};


template <class T>
std::ostream & operator <<(std::ostream & os, const graph<T> & G)
{
    os << G.n() << " " << G.m() << std::endl;
    for (auto v: G.V())
        os << v << " ";
    os << std::endl;

    for (auto v: G.V())
        for (auto w: G.Adj(v))
            if (v < w)
                os << v << " " << w << std::endl;
    return os;
}


template <class T>
std::istream &  operator >>(std::istream & is, graph<T> & G)
{
    std::size_t n, m;
    is >> n >> m;

    G = graph<T>();

    T v, w;
    for (std::size_t i = 0; i < n; ++i)
    {
        is >> v;
        G.addVertex(v);
    }

    for (std::size_t i = 0; i < m; ++i)
    {
        is >> v >> w;
        G.addEdge(v, w);
    }

    return is;
}


template <class GRAPH>
class Eulerian
{
public:

    typedef typename GRAPH::Vertex Vertex;
    typedef typename GRAPH::Path Path;

    Eulerian(const GRAPH & G)
    {
        _has_cycle = true;

        for (auto & v: G.V())
        {
            if (G.deg(v) % 2 == 1)
            {
                _has_cycle = false;
                break;
            }
        }
        _G = G;

    }

    Path ec()
    {
       if (!_has_cycle)
           return Path();

       Path ans;

       std::stack<Vertex> S;

       Vertex v = *(_G.V().begin());   // first vertex in V()
       S.push(v);

       while (!S.empty())
       {
           Vertex s = S.top();
           if (_G.deg(s) != 0)
           {
               Vertex w = *(_G.Adj(s).begin());  // first neighbor
               _G.removeEdge(s, w);
               S.push(w);

           }
           else // back to s
           {
               S.pop();
               ans.push_back(s);
           }
       }

       return ans;


    }
private:

    bool _has_cycle;  // true if the input graph has an Eulerian cycle
    GRAPH _G;
};



using namespace std;
graph<int> read_graph()
{
    int m, v, w;
    graph<int> G;

    cin >> m;
    for (int i = 0; i < m; ++i)
    {
        cin >> v >> w;
        G.addVertex(v);
        G.addVertex(w);
        G.addEdge(v, w); // change to multiset
    }

    return G;
}

int main (void) {
    int n = 0;
    cin >> n; // number of problems
    int i = 1;
    for (i = 1; i <= n; ++i) {
        graph<int> G = read_graph();
        Eulerian<graph<int>> E(G);
        graph<int>::Path p = E.ec();
        cout << "\nCase #" << i << "\n";
        if (p.empty()) {
            cout << "some beads may be lost" << endl;
        }
        else {
            for (i = 0; i < p.size()-1; ++i) {
                cout << p[i] << " " << p[i+1] << endl;
            }
            
        }
    }
}