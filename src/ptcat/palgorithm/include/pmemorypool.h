//
// Created by 27852 on 2026/3/13.
//

#ifndef PTCAT_PMEMORYPOOL_H
#define PTCAT_PMEMORYPOOL_H

#include<memory_resource>
#include <iostream>

namespace ptcat {
    namespace palgo {//算法命名空间
        class PMemoryPool{
        private:
            std::pmr::synchronized_pool_resource spr_;//内存池

        public:
            PMemoryPool() : spr_{[]{
                std::pmr::pool_options po;
                po.largest_required_pool_block = 1024 * 1024 * 50;//超过 50 M 的数据调用 new delete, 反之走内存池
                return po;
            }()}{
                std::cout << "memory pool init..." << std::endl;
            }

            ~PMemoryPool(){
                spr_.release();//显示调用释放全部内存，其实不调用也会自动释放
            }

            PMemoryPool(const PMemoryPool&) = delete;
            PMemoryPool& operator=(const PMemoryPool&) = delete;

            static PMemoryPool& Instance(){
                static PMemoryPool pmp;
                return pmp;
            }

            template<class T>
            T* Allocate(size_t buf_count, size_t alignment = alignof(T)){//alignment 带上对齐
                return static_cast<T*>(spr_.allocate(buf_count * sizeof(T), alignment));
            }

            template<class T>
            void DeAllocate(T* ptr, size_t buf_count, size_t alignment = alignof(T)){//删除对齐
                if (ptr)
                    spr_.deallocate(ptr, buf_count * sizeof(T), alignment);
            }

            void Release(){
                spr_.release();
            }
        };

        inline PMemoryPool& GetMP(){
            return PMemoryPool::Instance();
        };
    }
}

#endif //PTCAT_PMEMORYPOOL_H
