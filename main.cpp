#include <iostream>

using namespace std;

/**
 * Basic class to count the reference
 */
class ReferenceCounter{
private:
    int counter = 0;
public:
    void Increase(){
        counter++;
    }

    int Decrease(){
        return counter--;
    }

    int getCounter() const {
        return counter;
    }
};


/**
 * SmartPointer class that deals with memory
 * @tparam T
 */
template <typename T>
class SmartPointer{

private:
    T* object = nullptr;
    ReferenceCounter* referenceCounter = nullptr;

public:
    SmartPointer() = default;
    SmartPointer(T* object){
        string className = typeid(object).name();
        this->object = object;
        this->referenceCounter = new ReferenceCounter();
        referenceCounter->Increase();
        cout << "SmartPointer class is created for " << className << ". Counter: " << referenceCounter->getCounter() << endl;
    }

    virtual ~SmartPointer(){

        if(referenceCounter != nullptr){
            int tmp = referenceCounter->Decrease();
            cout << "-Destructor-. Reference count is: " << referenceCounter->getCounter() << endl;
            if(tmp <= 0){
                delete referenceCounter;
                delete object;
                referenceCounter = nullptr;
                object = nullptr;
            }
        }
    }
    /**
     * Copy constructor
     * @param other
     */
    SmartPointer(const SmartPointer<T>& other){
        object = other.object;
        referenceCounter = other.referenceCounter;
        referenceCounter->Increase();
        cout << "-Copy constructor-. Reference count is: " << referenceCounter->getCounter() << endl;
    }

    /**
     * Assignment operator. Checks whether the two objects are the same or not
     * @param other
     * @return
     *        the object
     */
    SmartPointer<T>& operator=(const SmartPointer<T>& other){
        if(this != &other){
            if(referenceCounter != nullptr && referenceCounter->Decrease() == 0){
                delete referenceCounter;
                delete object;
            }
            object = other.object;
            referenceCounter = other.referenceCounter;
            referenceCounter->Increase();
        }
        cout << "-Assignment operator-. Reference counter is: " << referenceCounter->getCounter() << endl;
        return *this;
    }
    T&operator*(){
        return *object;
    }
    T*operator->(){
        return object;
    }
};


/**
 * Demo class to test the SmartClass class.
 */
class DemoClass{

private:
    string name;
public:
    DemoClass(){
        cout << "basic class created" << endl;
    }
    DemoClass(string name){
        this->name = name;
        cout << name << " class created" << endl;
    }
    ~DemoClass(){
        cout << "Demo class destroyed." << endl;
    }
    void setName(string name){
        this->name = name;
        cout << "name changed to: " << name << endl;
    }


};

int main(){

    SmartPointer<DemoClass> demoClassPtr = new DemoClass();
    //counter is 1 to demoClass
    demoClassPtr->setName("NewName");
    SmartPointer<DemoClass> demoClassPtr2 = new DemoClass();
    //counter is 1
    demoClassPtr = demoClassPtr2;
    //counter is 2 to the second demoClass

    return 0;
}