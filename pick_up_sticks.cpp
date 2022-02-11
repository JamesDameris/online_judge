#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <vector>
#include <list>

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


template <class GRAPH>
class DFS
{

public:
    typedef typename GRAPH::Vertex Vertex;

    DFS() {
    }

    DFS(const GRAPH & G) {
        time = 0;
        _is_dag = true;

        for (auto & v: G.V()) {
            if (pre.count(v) == 0) {  // v has not been visited
                dfs_one(G, v);
            }
        }
    }

    void dfs_one(const GRAPH & G, const Vertex & v) {
        pre[v] = ++time;
        for (auto &w : G.Adj(v)) {
            if (pre.count(w) == 0) {
                dfs_one(G, w);
            }
            else if (pre.at(w) < pre.at(v) && post.count(w) == 0) { // proper back edge
                _is_dag = false;
            }
        }
        post[v] = ++time;
        
        _ts.push_front(v);
    }

    bool isDag() const {
        return _is_dag;
    }

    std::list<Vertex> ts() const {
        return _ts;
    }



private:
    std::unordered_map<Vertex, std::size_t> _C, pre, post;
    std::size_t time;
    bool _is_dag;
    std::list<Vertex> _ts;
};

using namespace std;

int main (void) {
    digraph<int> G;
    int M = 0, N = 0;
    int m = 0, n = 0;
    while (1) {
        cin >> N >> M;

        if (N == 0 && M == 0) {
            break;
        }
        for (int i=1; i<=N; ++i) {
            G.addVertex(i);
        }
        for (int i=1; i<=M; ++i) {
            cin >> n >> m;
            G.addEdge(n,m);
        }
        DFS<digraph<int> > D(G);
        if (!G.isDag()) {
            cout << "IMPOSSIBLE" << endl;
        }
        else {
            for (auto v: D.ts()) {
                cout << v << endl;
            }
        }
    }
}