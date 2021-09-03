#include <iostream>
#include <cstring>

// REF:
// Stepik, C/C++ multithreaded programming course, 2.3

// PROBLEM:
// > write a simple allocator that passes the tests in main()
// > no defragmentation required

class SmallAllocator
{
private:
    static constexpr int capacity = 1024u * 1024u;
    uint32_t offset = 0u;
    char* memory;
private:
    uint32_t get_block_size(void* block_start)
    {
        return *(static_cast<uint32_t*>(block_start) - 1);
    }
    void set_block_size(void* block_start, uint32_t size)
    {
        std::memcpy((void*)((uint32_t*)block_start - 1), (void*)(&size), sizeof(size));
    }
public:
    SmallAllocator()
        : memory{ new char[capacity] }
    {}
    ~SmallAllocator()
    {
        delete[] memory;
    }
    void* Alloc(uint32_t size)
    {
        uint32_t new_offset = offset + sizeof(uint32_t) + size;
        void* block_start = (void*)(memory + offset + sizeof(uint32_t));
        if (new_offset <= capacity)
        {
            set_block_size(block_start, size);
            offset = new_offset;
            return block_start;
        }
        else
            return nullptr;
    };
    void* ReAlloc(void* old_block, uint32_t size)
    {
        uint32_t prev_size = get_block_size(old_block);
        // if old_block is the last allocated block
        if (((char*)old_block - memory) + prev_size == offset)
        {
            set_block_size(old_block, size);
            return old_block;
        }
        else if (void* new_block = Alloc(size))
        {
            std::memcpy(new_block, old_block, prev_size);
            return new_block;
        }
        else
            return nullptr;
    };
    void Free(void* block) 
    {
        if (block)
        {
            uint32_t block_size = get_block_size(block);
            if (((char*)block - memory) + block_size == offset)
                offset -= (block_size + sizeof(uint32_t));

            std::memset(block, 0, block_size);
        }
        return;
    };
};

int main()
{
	// tests
    SmallAllocator A1;
    int* A1_P1 = (int*)A1.Alloc(sizeof(int));
    A1_P1 = (int*)A1.ReAlloc(A1_P1, 2 * sizeof(int));
    A1.Free(A1_P1);
    SmallAllocator A2;
    int* A2_P1 = (int*)A2.Alloc(10 * sizeof(int));
    for (unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
    for (unsigned int i = 0; i < 10; i++) if (A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;
    int* A2_P2 = (int*)A2.Alloc(10 * sizeof(int));
    for (unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
    for (unsigned int i = 0; i < 10; i++) if (A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
    for (unsigned int i = 0; i < 10; i++) if (A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;
    A2_P1 = (int*)A2.ReAlloc(A2_P1, 20 * sizeof(int));
    for (unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
    for (unsigned int i = 0; i < 20; i++) if (A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
    for (unsigned int i = 0; i < 10; i++) if (A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
    A2_P1 = (int*)A2.ReAlloc(A2_P1, 5 * sizeof(int));
    for (unsigned int i = 0; i < 5; i++) if (A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
    for (unsigned int i = 0; i < 10; i++) if (A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
    A2.Free(A2_P1);
    A2.Free(A2_P2);
	std::cout << "\nAll tests done. Press Enter to quit\n";
	std::cin.get();
    return 0;
}