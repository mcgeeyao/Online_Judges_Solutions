#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <tuple>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <limits>

#define ll long long
#define pii pair<int, int>

using namespace std;

// int main() {
//     cout.sync_with_stdio(false);
//     cin.tie(nullptr);
//     int t;
//     cin >> t;
//     while (t--) {
//         int n, k, res;
//         ll l;
//         char c;
//         string s;
//         cin >> n;
//         vector<int> arr(n);
//         for (int i = 0; i < n; ++i) cin >> arr[i];

//         cout << res << endl;
//     }
//     return 0;
// }


// Binary Index Tree
template<typename valueType>
class BIT {
private:
    vector<valueType> nums;
    vector<valueType> sums;
    valueType n;
public:
    BIT(valueType len) {
        n = len;
        nums.resize(n + 1, 0);
        sums.resize(n + 1, 0);
    }

    BIT(vector<valueType> arr) {
        n = arr.size();
        nums.resize(n + 1, 0);
        sums.resize(n + 1, 0);
        for(int i = 0; i < n + 1; ++i) Update(i, arr[i]);
    }

    bool Update(valueType ind, valueType val) {
        if (ind < 0 || ind >= n) return false;
        ind++;
        valueType diff = nums[ind] - val;
        nums[ind] = val;
        while (ind <= n) {
            sums[ind] -= diff;
            ind += (ind & -ind);
        }
        return true;
    }

    valueType Query(valueType ind) {
        if (ind < 0 || ind >= n) return 0;
        ind++;
        valueType sum = 0;
        while (ind > 0) {
            sum += sums[ind];
            ind -= (ind & -ind);
        }
        return sum;
    }
};


// Disjoint Set
template<typename valueType>
class DJS {
private:
    vector<valueType> arr;
    vector<valueType> rank;

public:
    DJS(valueType len) {
        arr.resize(len);
        for (valueType i = 0; i < len; ++i) arr[i] = i;
        rank.resize(len, 1);
    }

    valueType Find(valueType x) {
        if (arr[x] != x)
            arr[x] = find(arr[x]);
        return arr[x];
    }

    valueType Union(valueType x, valueType y) {
        valueType xp = find(x);
        valueType yp = find(y);
        if (xp != yp) {
            if (rank[xp] >= rank[yp]) {
                arr[yp] = xp;
                rank[xp] += rank[yp];
            } else {
                arr[xp] = yp;
                rank[yp] += rank[xp];
            }
        }
    }
};


// Segment Tree   
template<typename valueType>     
struct Segment_TreeNode {
    int lo, hi;
    valueType sum, min, max;
    Segment_TreeNode(int lo, int hi, valueType s, valueType mi, valueType ma): lo(lo), hi(hi), sum(s), min(mi), max(ma) {};
    Segment_TreeNode(int lo, int hi): lo(lo), hi(hi), sum(0), min(0), max(0) {};
};

template<typename valueType>         
class Segment_Tree {
private:
    vector< Segment_TreeNode<valueType>* > nodes;
    vector<bool> lazy;
    int n;
    vector<valueType> arr;

    void build(int ind, int lo, int hi) {
        if (lo == hi) {
            nodes[ind] = new Segment_TreeNode<valueType> (lo, hi, arr[lo], arr[lo], arr[lo]);
            return;
        }
        int mid = (lo + hi) / 2;
        int left = (ind << 1) + 1;
        int right = (ind << 1) + 2;

        build(left, lo, mid);
        build(right, mid + 1, hi);

        nodes[ind] = new Segment_TreeNode<valueType> (lo, hi);
        push_up(ind);
    }

    void push_up(int ind) {
        int left = (ind << 1) + 1;
        int right = (ind << 1) + 2;

        nodes[ind]->sum = nodes[left]->sum + nodes[right]->sum;
        nodes[ind]->min = min(nodes[left]->min, nodes[right]->min);
        nodes[ind]->max = max(nodes[left]->max, nodes[right]->max);
    }

    void push_down(int ind) {
        if (!lazy[ind]) return;
        int left = (ind << 1) + 1;
        int right = (ind << 1) + 2;

        nodes[left]->sum = nodes[ind]->max * (nodes[left]->hi - nodes[left]->lo + 1);
        nodes[left]->max = nodes[ind]->max;
        nodes[left]->min = nodes[ind]->min;
        
        nodes[right]->sum = nodes[ind]->max * (nodes[right]->hi - nodes[right]->lo + 1);
        nodes[right]->max = nodes[ind]->max;
        nodes[right]->min = nodes[ind]->min;
        
        lazy[ind] = false;
        lazy[left] = true;
        lazy[right] = true;
    }

    void _update(int ind, int l ,int r, valueType val) {
        int lo = nodes[ind]->lo;
        int hi = nodes[ind]->hi;
        int left = (ind << 1) + 1;
        int right = (ind << 1) + 2;

        if (r < lo || l > hi) return ;

        if (l <= lo && r >= hi) {
            nodes[ind]->sum = val * (hi - lo + 1);
            nodes[ind]->min = val;
            nodes[ind]->max = val;
            lazy[ind] = true;
        } else {
            push_down(ind);
            _update(left, l, r, val);
            _update(right, l, r, val);
            push_up(ind);
        }
    }

    tuple<valueType, valueType, valueType> _query(int ind, int l, int r) {
        int lo = nodes[ind]->lo;
        int hi = nodes[ind]->hi;
        int left = (ind << 1) + 1;
        int right = (ind << 1) + 2;

        if (r < lo || l > hi)
            return make_tuple(0, numeric_limits<valueType>::max(), numeric_limits<valueType>::min());
        if (l <= lo && r >= hi)
            return make_tuple(nodes[ind]->sum, nodes[ind]->min, nodes[ind]->max);
            
        push_down(ind);

        valueType s1, mi1, ma1, s2, mi2, ma2;
        tie(s1, mi1, ma1) = _query(left, l, r);
        tie(s2, mi2, ma2) = _query(right, l, r);
            
        return make_tuple(s1 + s2, min(mi1, mi2), max(ma1, ma2));
    }
public:

    Segment_Tree(int num) {
        n = num;
        nodes.resize(4*n);
        lazy.resize(4*n);
        arr.resize(n, 0);
        build(0, 0, n - 1);
    }

    Segment_Tree(vector<valueType> _arr) {
        n = _arr.size();
        nodes.resize(4*n);
        lazy.resize(4*n);
        arr = _arr;
        build(0, 0, n - 1);
    }

    void update(int l,int r,valueType val) {_update(0, l, r, val);}
    void update(int l, valueType val) {_update(0, l, l, val);}
    valueType query_max(int l,int r) {return get<2>(_query(0, l, r));}
    valueType query_min(int l,int r) {return get<1>(_query(0, l, r));}
    valueType query_sum(int l,int r) {return get<0>(_query(0, l, r));}

};