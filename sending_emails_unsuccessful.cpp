// James Dameris Online Judge Problem 10986: Sending Emails
// 3/7/2022
#include <limits>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>


template <class GRAPH> class BFS;

template <class T>
struct Edge
{
    T a, b;

    Edge() {}

    Edge(const T & a, const T & b): a(a), b(b)
    {}

};


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

template <class T>
bool operator <(const Edge<T> &e1,
                const Edge<T> &e2)
{

    return ((e1.a < e2.a) || ((e1.a == e2.a) && (e1.b < e2.b)));
}

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


private:
};

template <class T>
struct WEdge: public Edge<T>
{
    double w;

    WEdge();

    WEdge(const T &a, const T &b, double w): Edge<T>(a, b), w(w)
    {}
};

template <class T>
bool operator < (const WEdge<T> & e1, const WEdge<T> & e2)
{
    return ((e1.w < e2.w) ||
            ((e1.w == e2.w) && (Edge<T>(e1) < Edge<T>(e2))));
}

template <class T>
class wgraph: public graph<T>
{
  public:
    typedef T Vertex;

    wgraph()
    {

    }

    void addEdge(const T & a, const T & b, double w)
    {
        graph<T>::addEdge(a, b);
        _w[Edge<T>(a, b)] = _w[Edge<T>(b, a)] = w;
    }

    void addEdge(const Edge<T> & e, double w)
    {
        addEdge(e.a, e.b, w);
    }

    void removeEdge(const T & a, const T & b)
    {
        graph<T>::removeEdge(a, b);
        _w.erase(Edge<T>(a, b));
        _w.erase(Edge<T>(b, a));
    }

    double cost(const T & a, const T & b) const
    {
        return _w.at(Edge<T>(a, b));

    }

    double cost(const Edge<T> & e) const
    {
        return cost(e.a, e.b);
    }

    std::set<WEdge<T>> E() const
    {
        std::set<WEdge<T>> ans;

        for (auto &v: graph<T>::V())
            for (auto & w: graph<T>::Adj(v))
                ans.insert(WEdge<T>(v, w, cost(v, w)));

        return ans;

    }

private:
    std::map<Edge<T>, double> _w;   // maps an edge to its weight
};

template <class T>
class network: public digraph<T>
{

public:
    network()
    {

    }

    void addEdge(const T & a, const T & b, double w)
    {
        digraph<T>::addEdge(a, b);
        _w[{a, b}] = w;
    }

    void addEdge(const WEdge<T> & e)
    {
        digraph<T>::addEdge(e.a, e.b);
        _w[{e.a, e.b}] = e.w;
    }

    double cost(const T & a, const T & b) const
    {
        assert(digraph<T>::isEdge(a, b));
        return _w.at({a, b});
    }

    void setCost(const T & a, const T & b, double w)
    {
        assert(digraph<T>::isEdge(a, b));
        _w[{a, b}] = w;

    }

    std::set<WEdge<T>> E() const
    {
        std::set<WEdge<T>> ans;

        for (auto & a: digraph<T>::V())
            for (auto &b: digraph<T>::Adj(a))
                ans.insert({a, b, cost(a, b)});
    }

private:
    std::map<Edge<T>, double> _w;

};


template <class T>
class dary_heap
{
public:
    dary_heap(std::size_t d = 2): _d(d)
    {
        _n = 0;
    }

    std::size_t size() const
    {
        return _n;
    }

    T min() const
    {
        assert(_n != 0);
        return _data[0];
    }

    bool empty() const
    {
        return (_n == 0);
    }

    void push(const T & x)
    {

        assert(_l.count(x) == 0);   //  x is not already in heap
        if (_n == _data.size())     //  vector is full
            _data.push_back(x);
        else
            _data[_n] = x;


        // record where x lives in _data and increment the key count
        _l[x] = _n;
        ++_n;

        std::size_t i = _l.at(x);
        while (i > 0)
        {
            std::size_t parent = (i - 1) / _d;
            if (_data[i].second < _data[parent].second)
            {
                
                std::swap(_data[i], _data[parent]);
                _l[_data[i]] = i;
                _l[_data[parent]] = parent;
                i = parent;
                

            }
            else
                break;
        }

    }

    void decrease_key(const T & x, const T & newx)
    {

        // new key is smaller than old key;  old key is in heap; new key is not yet in heap
        
        assert(newx < x && _l.count(x) != 0 && _l.count(newx) == 0);
        
        std::size_t i = _l.at(x);
        
        _data[i] = newx;
        
        _l.erase(x);
        
        _l[newx] = i;


        while (i > 0 && _data[i].second < _data[(i-1)/_d].second)
        {
            std::size_t parent = (i-1)/_d;

            std::swap(_data[i], _data[parent]);
            _l[_data[parent]] = parent;
            _l[_data[i]] = i;
            i = parent;
        }

    }

    void pop_min()
    {

        assert(!empty());

        _l.erase(_data[0]);
        if (--_n == 0)
            return;

        _data[0] = _data[_n];
        _l[_data[0]] = 0;
        std::size_t i = 0;

        while (i*_d + 1 < _n)      //  there is at least one child
        {
            std::size_t left(i*_d + 1), right((i+1)*_d), m(left);

            for (std::size_t c = left; c < _n && c < right; ++c)
                if (_data[c] < _data[m])
                    m = c;


            // if smallest child is greater or equal to parent, then heap order has been restored
            if (!(_data[m] < _data[i]))
                break;

            // otherwise swap parent and minimum child
            std::swap(_data[i], _data[m]);
            _l[_data[i]] = i;
            _l[_data[m]] = m;
            i = m;
        }
    }

private:
    std::size_t _n;                   // number of elements in heap
    std::size_t _d;                   // number of children
    std::vector<T> _data;             // heap elements live here
    std::map<T, std::size_t> _l;      // _data[_l[key]] = key

};


template <class Graph>
class dijkstra
{
public:
    typedef typename Graph::Vertex Vertex;
    typedef std::pair<Vertex, double> PAIR;
    dijkstra() {};

    std::map<Vertex, Vertex> d(Graph G, Vertex s)
    {
        for (auto v: G.V())
        {
            _d[v] = std::numeric_limits<double>::infinity();
        }
        _d[s] = 0;
        size_t comp = (2 >= G.m()/G.n() ? 2 : G.m()/G.n());
        dary_heap<PAIR> H(comp);
        for (auto v: G.V()) 
        {
            H.push(PAIR(v,_d[v]));
        }

        for (int i = 0; i < G.V().size(); ++i)
        {
            PAIR v = H.min();
            _deleted.insert(v.first);
            H.pop_min();
                
            for (auto w: G.Adj(v.first))
            {
                if (_deleted.count(w) == 0 && _d[w] > _d[v.first] + G.cost(v.first, w))
                {
                    double weight = _d[w];
                    _d[w] = _d[v.first] + G.cost(v.first, w);
                    _parent[w] = v.first;
                    H.decrease_key(PAIR(w,weight), PAIR(w,_d[w]));
                }
            }
        }
        return _parent;
    }

    friend bool operator < (const PAIR & p1, const PAIR & p2)
    {
        return (p1.second < p2.second);
    }

private:
    std::map<Vertex, Vertex> _parent;
    std::map<Vertex, double> _d;
    std::unordered_set<Vertex> _deleted;
};

using namespace std;
int main()
{
    size_t n, m, numProblems;
    int start, end, s, t;
    double c;
    cin >> numProblems;
    for (int i = 0; i < numProblems; ++i) 
    {
        network<int> G;
        cin >> n >> m >> start >> end;
        if ((start == end) || (m == 0))
        {
            cout << "Case #" << (i+1) << ": " << "unreachable" << endl;
            continue;
        }
        for (size_t i = 0; i < n; ++i)
        {
            G.addVertex(i);
        }

        for (size_t i = 0; i < m; ++i)
        {
            cin >> s >> t >> c;
            G.addEdge(s, t, c);
            G.addEdge(t, s, c);
        }
        dijkstra<network<int> > D;

        map<int, int> parent = D.d(G, start);
        double TOTAL = 0;
        int cursor = start;
        for (auto p : parent)
        {   
            TOTAL += G.cost(p.first,p.second);
        }
        if (TOTAL == 0)
            cout << "Case #" << (i+1) << ": " << "unreachable" << endl;
        else 
            cout << "Case #" << (i+1) << ": " << TOTAL << endl;
    }
    return 0;
}