#include <iostream>
#include <memory>
#include <cstring>
#include <fstream>
#include "TaskRunner.cpp"


class ResourceManager{
private:
    int* data_;
public:
    ResourceManager(int value) : data_(new int(value)){}
    
    ~ResourceManager(){
        delete data_;
    }
    
    ResourceManager(const ResourceManager&) = delete;
    
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    ResourceManager(ResourceManager&& other) noexcept : data_(other.data_){
        other.data_ = nullptr;
    }
    
    ResourceManager& operator=(ResourceManager&& other) noexcept {
        if(this != &other){
            delete data_;
            data_ = other.data_;
            other.data_ = nullptr;
        }
        return *this;
    }
    
};



class COW{
private:
    struct Buffer{
        size_t size;
        std::unique_ptr<int[]> data;
        
        Buffer(size_t s) : size(s), data(new int[size]){
            std::memset(data.get(), 0, size * sizeof(int));
        }
    };
    
    std::shared_ptr<Buffer> buffer_;
    
    void ensure_unique(){
        if(buffer_.use_count() > 1){
            std::shared_ptr<Buffer> newBuffer = std::make_shared<Buffer>(buffer_->size);
            std::memcpy(newBuffer->data.get(), buffer_->data.get(), buffer_->size * sizeof(int));
            buffer_ = newBuffer;
        }
    }
public:
    explicit COW(size_t size) : buffer_(std::make_shared<Buffer>(size)){}
    
    COW() = default;
    
    COW(const COW&) = default;
    
    COW& operator=(const COW&) = default;
    
    void write(size_t index,int value){
        ensure_unique();
        if(buffer_->size > index){
            buffer_->data[index] = value;
        }
    }
    
    int read(size_t index){
        if(buffer_->size > index){
            return buffer_->data[index];
        }
        return -1;
    }
    
};

template <typename T>
class CountAllocator : public std::allocator<T>{
private:
    inline static size_t alloc_count = 0;
    inline static size_t dealloc_count = 0;
public:
    using Base = std::allocator<T>;
    using value_type = T;
    
    CountAllocator() = default;
    
    template <typename U>
    CountAllocator(const CountAllocator<U>&) {};
    
    T* allocate(size_t n){
        ++alloc_count;
        std::cout << "[Alloc]: " << alloc_count << std::endl;
        return Base::allocate(n);
    }
    
    void deallocate(T* p,size_t n){
        ++dealloc_count;
        std::cout << "[Dealloc]: " << dealloc_count << std::endl;
        Base::deallocate(p, n);
    }
};


template <typename T,size_t N = 1024>
class MemoryAllocator{
private:
    T* pool;
    std::vector<T*> free_list;
public:
    using value_type = T;
    
    MemoryAllocator() {
        pool = static_cast<T*>(::operator new(N * sizeof(T)));
        for(size_t i = 0;i < N;++i){
            free_list.push_back(pool + i);
        }
    }
    
    ~MemoryAllocator(){
        ::operator delete(pool);
    }
    
    T* allocate(size_t n){
        if(n != 1 || free_list.empty()) throw std::bad_alloc();
        
        T* p = free_list.back();
        free_list.pop_back();
        return p;
    }
    
    void deallocate(T* p,size_t n){
        free_list.push_back(p);
    }
};

template <typename T1,typename T2,size_t N>
bool operator==(const MemoryAllocator<T1,N>&,const MemoryAllocator<T2,N>&) { return true; }

template <typename T1,typename T2,size_t N>
bool operator!=(const MemoryAllocator<T1,N>&,const MemoryAllocator<T2,N>&) { return false; }


template <typename Primary,typename FallBack>
class FallBackAllocator{
private:
    Primary primary_;
    FallBack fallback_;
public:
    using value_type = typename Primary::value_type;
    
    FallBackAllocator() = default;
    
    template <typename U>
    FallBackAllocator(const FallBackAllocator<U,FallBack>&) {}
    
    value_type* allocate(size_t n){
        try {
            return primary_.allocate(n);
        } catch (const std::bad_alloc&) {
            return fallback_.allocate(n);
        }
    }
    
    void deallocate(value_type* p,size_t n){
        try {
            primary_.deallocate(p,n);
        } catch (...) {
            fallback_.deallocate(p,n);
        }
    }
};


template <typename T,size_t Aligment>
struct AligmentAllocator{
    using value_type = T;
    
    AligmentAllocator() = default;
    
    template <typename U>
    AligmentAllocator(const AligmentAllocator<U,Aligment>&) {}
    
    T* allocate(size_t n){
        void* ptr = nullptr;
        if(posix_memalign(&ptr, Aligment, n * sizeof(T)) != 0) throw std::bad_alloc();
        return static_cast<T*>(ptr);
    }
    
    void deallocate(T* p,size_t n){
        free(p);
    }
    
};


std::vector<unsigned char> decode(const std::string& hex){
    std::vector<unsigned char> bytes;
    for(size_t i = 0;i < hex.length();i+=2){
        std::string byteStr = hex.substr(i,2);
        unsigned char byte = static_cast<unsigned char>(strtol(byteStr.c_str(),nullptr,16));
        
        bytes.push_back(byte);
    }
    return bytes;
}


std::string encode(const std::string& text){
    std::string hex;
    
    for(unsigned char c : text){
        char buf[3];
        
        snprintf(buf, sizeof(buf), "%02x",c);
        
        hex += buf;
    }
    return hex;
}

int main() {
    
    return 0;
}
