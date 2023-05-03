#include <memory>

template <class T, std::size_t growRate = 1024>
class MemoryPool {

    struct Block {
        Block* next;
    };

    class Buffer {
        static const std::size_t blockSize = sizeof(T) > sizeof(Block) ? sizeof(T) : sizeof(Block);
        uint8_t data[blockSize * growRate];

    public:
        Buffer*const next;
        Buffer(Buffer* next) : next(next) {}

        T* getBlock(std::size_t index) {
            return reinterpret_cast<T*>(&data[blockSize * index]);
        }
    };

    Block* firstFreeBlock = nullptr;
    Buffer* firstBuffer = nullptr;
    std::size_t bufferedBlocks = growRate;

public:

    MemoryPool() = default;

    ~MemoryPool() {
        while (firstBuffer) {
            Buffer* buffer = firstBuffer;
            firstBuffer = buffer->next;
            delete buffer;
        }
    }

    T *allocate() {
        if (firstFreeBlock) {
            Block* block = firstFreeBlock;
            firstFreeBlock = block->next;
            return reinterpret_cast<T*>(block);
        }

        if (bufferedBlocks >= growRate) {
            firstBuffer = new Buffer(firstBuffer);
            bufferedBlocks = 0;
        }

        return firstBuffer->getBlock(bufferedBlocks++);
    }

    void deallocate(T* pointer) {
        auto* block = reinterpret_cast<Block*>(pointer);
        block->next = firstFreeBlock;
        firstFreeBlock = block;
    }
};

template <class T, std::size_t growRate = 1024>
class PoolAllocator : private MemoryPool<T, growRate> {
public:

    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    using value_type = std::pair<T, size_t>;

    template <class U>
    struct rebind {
        typedef PoolAllocator<U, growRate> other;
    };

    pointer allocate(size_type n, const void* hint = nullptr) {
        if (n != 1 || hint) throw std::bad_alloc();

        return MemoryPool<T, growRate>::allocate();
    }

    void deallocate(pointer p, size_type n) {
        MemoryPool<T, growRate>::deallocate(p);
    }

    void construct(pointer p, const_reference val) {
        new (p) T(val);
    }

    void destroy(pointer p) {
        p->~armies();
    }
};
