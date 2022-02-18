#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <vector>
#include <list>
#include <stack>


template <class T>
class graph
{

public:

    typedef T Vertex;
    typedef std::unordered_set<T> VertexSet;
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
        return (isVertex(v) && isVertex(w) && _t.at(v).count(w) == 1);
    }

    VertexSet Adj(const T & v) const
    {
        assert(isVertex(v));
        return _t.at(v);
    }

    std::size_t deg(const T & v) const
    {
        assert(isVertex(v));
        return _t.at(v).size();
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
        _t[v].insert(w);
        _t[w].insert(v);
    }

    void removeEdge(const T & v, const T & w)
    {
        if (!isEdge(v, w))
            return;

        _t[v].erase(w);
        _t[w].erase(v);
    }



protected:

    std::unordered_map<Vertex, VertexSet> _t;

};


template <class T> class DFS;

template <class T>
class digraph: public graph<T>
{

public:
    typedef T Vertex;

    std::size_t m() const
    {
        std::size_t ans(0);

        for (auto &v: graph<T>::V())
            ans += graph<T>::Adj(v).size();
        return ans;
    }

    std::size_t indeg(const Vertex & v) const
    {
        std::size_t ans(0);

        for (auto & w: graph<T>::V())
        {
            ans += graph<T>::Adj(w).count(v);
        }
        return ans;
    }

    std::size_t outdeg(const Vertex & v) const
    {
        return graph<T>::Adj(v).size();
    }

    void addEdge(const Vertex & v, const Vertex & w)
    {
        assert(graph<T>::isVertex(v) && graph<T>::isVertex(w));
        graph<T>::_t[v].insert(w);
    }

    void removeEdge(const Vertex &v, const Vertex & w)
    {
        if (!graph<T>::isEdge(v, w))
            return;

        graph<T>::_t[v].erase(w);
    }

    bool isDag() const
    {
        DFS<digraph<Vertex>> D(*this);
        return D.isDag();
    }

    digraph reverse() const
    {
        digraph ans;

        for (auto &v: graph<Vertex>::V())
            ans.addVertex(v);

        for (auto &v: graph<Vertex>::V())
        {
            for (auto &w: graph<Vertex>::Adj(v))
                ans.addEdge(w, v);
        }

        return ans;
    }

private:

};


template <class DIGRAPH>
class TSCC
{
public:

    typedef typename DIGRAPH::Vertex Vertex;
    typedef typename DIGRAPH::VertexSet VertexSet;

    TSCC(const DIGRAPH & G)
    {
        _time = _cid = 0;
        for (auto &v : G.V())
        {
            if (_pre.count(v) == 0)
                tdfs_one(G, v);
        }
    }

    std::size_t numComp() const 
    { 
        return _cid;
    }


    std::unordered_map<Vertex, std::size_t> component() const
    {
        return _C;
    }

private:

    void tdfs_one(const DIGRAPH & G, const Vertex & v)
    {
        _pre[v] = _low[v] = _time++;
        _S.push(v);

        for (auto &w: G.Adj(v))
        {
            if (_pre.count(w) == 0) // tree edge
            {
                tdfs_one(G, w);
                _low[v] = std::min(_low.at(v), _low.at(w));
            }

            // check to see if it is a back edge
            else if (_pre.at(w) < _pre.at(v) && _post.count(w) == 0)// back edge
                _low[v] = std::min(_low.at(v), _pre.at(w));
        }
        _post[v] = _time++;
        if (_pre.at(v) == _low.at(v))  // root of a component
        {
            Vertex top;
            do
            {
                top = _S.top();
                _S.pop();
                _C[top] = _cid;
            } while (top != v);
            ++_cid;
        }
    }



    std::unordered_map<Vertex, std::size_t> _pre, _post, _low, _C;
    std::size_t _time, _cid;
    std::stack<Vertex> _S;




};


using namespace std;

int main(void) 
{
    int cases = 0;
    int x = 0,y = 0;
    int n = 0, m = 0;
    cin >> cases;
    for (int i = 0; i < cases; i++) 
    {
        digraph<int> G;
        cin >> n >> m; 
        for (int j=1; j<=n; ++j) 
        { // add vertices from 1 to N
            G.addVertex(j);
        }
        for (int j=1; j<=m; ++j) 
        { // add edges from 1 to M
            cin >> x >> y; // edge 1 and 2
            G.addEdge(x,y); 
        }
        TSCC<digraph<int> > T(G);
        unordered_map<digraph<int>::Vertex, std::size_t> _C = T.component();
        unordered_set<int> not_source;
        for (auto v: G.V()) {
            for (auto w: G.Adj(v)) {
                if (_C.at(v) != _C.at(w)) {
                    not_source.insert(_C.at(w));
                }
            }
        }
        cout << T.numComp() - not_source.size() << endl;
    }
}