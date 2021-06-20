/*

        vector      list 
search  √           ×
insert  ×           √
remove

树结构：半线性结构

应用：
表达式 rpn

        *
      +   3
    1  2

数学：
特殊的图 T = (V, E), 节点数|V| = n, 边数|E| = e
一组元素之间的二元的关系，有关系引入一条边edge， 点vertex
每个vertex以node的形式在计算机中表示和实现。

图论与计算机数区别：
指定任意节点r ∈ V 作为跟root， T即成为有根树， rooted tree

若T1,T2,..Td为有根树
则 T = ((∪Vi)∪{r}, (∪E)∪{<r,ri>| 1≤　i <ｄ})也是
相对于T, Ti称为以ri为跟的子树(subtree rooted at ri), 记作Ti = subtree(ri)


ri为r的孩子(child),ri之间互称兄弟(sibling)
r为ri的父亲(parent),  d = degree(r)为r的（出）度数

兄弟之间的长幼次序，计算机中怎么区分呢

归纳法证明 e = Σ r∈V degree(r) = n -1 = O(n)
如果以 n+e 表示一颗树的规模，其规模可以看作O(n)或者O(e)

故在衡量相关复杂度时，可以n做作为参照


兄弟之间的关系，是计算机中的树与数学中的树区别较大的地方
若是指定Ti作为T的第i棵子树，ri作为r的第i个孩子，则T称作有序树(ordered tree)



路径+环路：
V中的k+1个节点，通过E中的k条边依次相联，构成一条路径（path）,亦称通路
    π = {(v0,v1), (v1,v2), ..., (vk-1, vk)}

路径长度：|π| = 边数 = k

环路(cycle/loop): vk = v0


连通+无环：
节点之间均有路径，称作连通图(connected)
不含环路，称作无环图(acyclic)

树：    无环连通图          ≤ e ≤
        极小连通图
        极大无环图

故：任一节点v与跟之间存在唯一路径
path(v,r) = path(v)     // 省略掉v


深度+层次：
不致歧义，路径、节点和子树可相互指代
    path(v)~v~subtree(v)
    path from root to v
    subtree form root to v
v的深度： depth(v) = |path(v)|

path(v)上的节点，均为v的祖先(ancestor)
v是它们的后代(descendent)

其中，除自身以外，是真(proper)祖先/后代

半线性:在任一深度
v的祖先/后代若存在，则必然/未必唯一
    前驱看作pre，后继suc         前驱唯一性可以，后继不行

跟节点是所有节点的公共祖先，深度为0

没有后代的节点称作叶子(leaf)

所有叶子深度中的最大者，称作(子)树(跟)的高度
height(v) = height(subtree(v))

对于一个子树，其高度可以称之为子树跟 节点的高度

退化的树高度为0

特别地，空数的高度取作-1

depth(v)  + height(v) ≤ height(T)

接口：
节点            功能
root()          根节点
parent()        父节点
firstChild()    长子
nextSibling()   兄弟
insert(i, e)    将e作为第i个还在插入
remove(i)       删除第i个还在(及其后代)
traverse()      遍历

父节点：
rank   data   parent
rank表示秩，编号
空间性能: O(n)
时间性能：      parent()    : O(1)
                root()     : O(1)或者O(n)
                firstChild : O(n)     
               nextSibling : O(n)



长子+兄弟

tree
bintree 是tree子集，通过长子兄弟法可以表示全部的树
*/

#include <stdio.h>
#include <queue>

#define MAX(p1,p2) (((p1) > (p2)) ? (p1) : (p2))

class Tree{
public:
    Tree(){}
    Tree(void *data, Tree *parent, Tree *firstChild, Tree *nextSibling):
        data(data), parent(), firstChild(firstChild),nextSibling(nextSibling){}

    Tree* getParent() {
        return parent;
    }

    Tree* getFirstChild() {
        return firstChild;
    }

    Tree* getNextSibling() {
        return nextSibling;
    }

    int getSize() {
        int size = 1;
        Tree* subtree = firstChild;
        while (subtree) {
            size += subtree->getSize();
            subtree = subtree->getNextSibling();
        }
        return size;
    }

    int getHeight() {
        int height = -1;
        Tree* subtree = firstChild;
        while (subtree) {
            height = MAX(height, subtree->getHeight());
            subtree = subtree->nextSibling;
        }
        return height+1;
    }

    int getDepth() {
        int depth = 0;
        Tree *p = parent;
        while (p) {
            depth++;
            p = p->parent;
        }
        return depth;
    }

    // first child -> order = 1
    bool insert(int order, Tree* child) {
        if (order <= 0) false;
        child->parent == this;

        if (1 == order || NULL == firstChild) {
            child->nextSibling = firstChild;
            firstChild = child;
        }

        Tree *bigerSibling = firstChild;
        order--;
        while (order-- && bigerSibling) {
            if (NULL == bigerSibling->nextSibling) {
                break;
            }

            bigerSibling = bigerSibling->nextSibling;
        }

        child->nextSibling = bigerSibling->nextSibling;
        bigerSibling->nextSibling = child;
        return true;
    }

    void remove(int order) {
        if(order < 0 || NULL == firstChild)
            return;

        Tree *bigerSibling = firstChild;
        if(1 == order) {
            firstChild = firstChild->nextSibling;
            delete bigerSibling;
            return;
        }
        order -= 2;
        while (order-- && bigerSibling) {
            if (NULL == bigerSibling->nextSibling) {
                return;
            }
            bigerSibling = bigerSibling->nextSibling;
        }
        Tree *tempTree = bigerSibling->nextSibling;
        bigerSibling->nextSibling = bigerSibling->nextSibling->nextSibling;
        delete tempTree;
    }

    void* traverse() {
        Tree *root = this;
        std::queue<Tree*> que;
        que.push(root);
        while(!que.empty()) {
            root = que.back();
            que.pop();
            while (root) {
                Tree *subtree = root->firstChild;
                while(subtree) {
                    que.push(subtree);
                    subtree = subtree->nextSibling;
                }
                // print root->data
            }
        }
    }

    void* traverse_dfs() {
        Tree *subtree = firstChild;

        while(subtree) {
            subtree->traverse_dfs();
            subtree = subtree->firstChild;
        }
        // print root->data
        return;
    }
private:
    void* data;
    Tree *parent, *firstChild, *nextSibling;
};

int main() {

}

