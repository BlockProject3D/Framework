#ifndef TREENODE_H_
# define TREENODE_H_

namespace Framework
{
    template <typename T>
    class ENGINE_API FTreeNode
    {
    private:
        T Value;
        FTreeNode<T> *Left;
        FTreeNode<T> *Right;

        inline uint32 GetNodeCountRec(const FTreeNode<T> *node) const
        {
            if (node == Null)
                return (0);
            uint32 v = GetNodeCountRec(node->Left);
            uint32 v1 = GetNodeCountRec(node->Right);
            return (1 + v + v1);
        }
    public:
        inline FTreeNode()
            : Left(Null), Right(Null)
        {
        }

        inline uint32 GetNodeCount() const
        {
            return (GetNodeCountRec(this));
        }

        inline uint32 GetTreeDepth() const
        {
            return (0);
        }

        inline void SetLeftNode(const T &val)
        {
            Left = new FTreeNode<T>;
            Left->Value = val;
        }

        inline void SetRightNode(const T &val)
        {
            Right = new FTreeNode<T>;
            Right->Value = val;
        }

        inline void ClearLeftNode()
        {
            delete Left;
            Left = Null;
        }
        
        inline void ClearRightNode()
        {
            delete Right;
            Right = Null;
        }
    };
};

#endif /* TREENODE_H_ */
