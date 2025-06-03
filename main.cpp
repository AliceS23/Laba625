#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "Pattern.h"
using namespace std;

enum class DogBread : int
{
   York = 1,
   Spaniel = 2,
   Alabai = 3,
   Undefined = 0
};
enum class DogSize : int
{
    Small,
    Medium,
    Big,
    Unknown
};

class Dogs
{
private:
    DogBread Bread;
    DogSize Size;
protected:
    string Name;
    bool DogIsDirty;
public:
    Dogs(DogBread Bread,DogSize Size);
    virtual ~Dogs() {}
    virtual void Wash();
    bool IsDirty() const {return DogIsDirty; }
    DogBread GetBread() const { return Bread; }
    DogSize GetSize() const { return Size; }
};

Dogs::Dogs(DogBread bread,DogSize dsize) :Bread(bread), Size(dsize)
{
    DogIsDirty = static_cast<bool>(rand()%2);
}

void Dogs::Wash()
{
    if(IsDirty())
    {
        cout << "Wash a DIRTY dog...";
    }
    else
    {
        cout << "Wash a CLEAN dog...";
    }
}

class York : public Dogs
{
public:
    York();
    ~York() {};
    void Wash() override;

};
class Alabai: public Dogs
{
public:
    Alabai();
    ~Alabai() {};
    void Wash() override;
};
class Spaniel : public Dogs
{
private:
public:
    Spaniel();
    ~Spaniel() {};
    void Wash() override;
};

Spaniel::Spaniel() : Dogs(DogBread::Spaniel,DogSize::Medium)
{
    Name = "Jackson";
}

York::York() : Dogs(DogBread::York,DogSize::Small)
{
    Name = "Milka";
}

Alabai::Alabai() : Dogs(DogBread::Alabai,DogSize::Big)
{
    Name = "Jazzy" ;
}

void York::Wash()
{
    Dogs::Wash();
    cout<< "Wash the York...";
}
void Spaniel::Wash()
{
    Dogs::Wash();
    cout<< "Wash the Spaniel...";
}
void Alabai::Wash()
{
    Dogs::Wash();
    cout<< "Wash the Alabai...";
}
// "Фабричный метод" для создания объектов собак

Dogs *CreateDogs(DogBread bread)
{
    Dogs *newDog = nullptr;
    if(bread == DogBread::York)
    {
        newDog = new York;
    }
    else if (bread == DogBread::Spaniel)
    {
        newDog = new Spaniel;
    }
    else if (bread == DogBread::Alabai)
    {
        newDog = new Alabai;
    }
    return newDog;
}
// Декоратор итератора для выделения собак по породе
class DogBreadDecorator : public IteratorDecorator<class Dogs*>
{
private:
    DogBread TBread;

public:
    DogBreadDecorator(Iterator<Dogs*> *iter, DogBread bread)
    : IteratorDecorator<Dogs*>(iter), TBread(bread) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetBread() != TBread)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->GetBread() != TBread);
    }
};
// Декоратор итератора для выделения собак по размеру
class DogSizeDecorator : public IteratorDecorator<class Dogs*>
{
private:
    DogSize TSize;

public:
    DogSizeDecorator(Iterator<Dogs*> *iter, DogSize dsize)
    : IteratorDecorator<Dogs*>(iter), TSize(dsize) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetSize() != TSize)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->GetSize() != TSize);
    }
};
// Декоратор итератора для выделения "грязных" или "чистых" собак
class DogDirtyDecorator : public IteratorDecorator<class Dogs*>
{
private:
    bool TDirty;

public:
    DogDirtyDecorator(Iterator<Dogs*> *it, bool isDirty)
    : IteratorDecorator<Dogs*>(it), TDirty(isDirty) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->IsDirty() != TDirty)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->IsDirty() != TDirty);
    }
};
// Функция, позволяющая "помыть" любую собаку из любого контейнера
void WashEmAll(Iterator<Dogs*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Dogs *currentDogs = it->GetCurrent();
        currentDogs->Wash();
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");
    size_t N = 27;
    ArrayClass<Dogs*> dogsArray;
    for(size_t i=0; i<N; i++)
    {
        int dog_num = rand()%3+1;
        DogBread dog_bread = static_cast<DogBread>(dog_num);
        Dogs *newDog = CreateDogs(dog_bread);
        dogsArray.Add(newDog);
    }

    wcout << L"Размер массива собак: " << dogsArray.Size() << endl;

    vector<Dogs*> dogsVector;
    for (size_t i =0; i<N;i++)
    {
        int dog_num = rand()%3+1;
        DogBread dog_bread = static_cast<DogBread>(dog_num);
        Dogs *newDog = CreateDogs(dog_bread);
        dogsVector.push_back(newDog);
    }

    wcout << L"Размер вектора собак: " << dogsVector.size() << endl;

    cout << endl << "wash everyone in a simple loop:" << endl;
    for(size_t i=0; i<dogsArray.Size(); i++)
    {
        Dogs *currentDog = dogsArray[i];
        currentDog->Wash();
    }
    cout << endl;

    // Моем всех при помощи итератора
    cout << endl << "wash everyone using iterator:" << endl;
    Iterator<Dogs*> *eIt = dogsArray.GetIterator();
    WashEmAll(eIt);
    delete eIt;
    cout << endl;

   // Моем всех грязных собак
    cout << endl << "Wash everyone dirty dogs using iterator:" << endl;
    Iterator<Dogs*> *dirtyIt = new DogDirtyDecorator(dogsArray.GetIterator(),true);
    WashEmAll(dirtyIt);
    delete dirtyIt;
    cout << endl;

// Моем всех Алабаев
    cout << endl << "Wash everyone Alabai using iterator:" << endl;
    Iterator<Dogs*> *eAlabai = new DogBreadDecorator(dogsArray.GetIterator(),DogBread::Alabai);
    WashEmAll(eAlabai);;
    delete eAlabai;
    cout << endl;

// Моем всех грязных Алабаев
    cout << endl << "Wash everyone dirty Alabai using iterator:" << endl;
    Iterator<Dogs*> *eDirtyAlabai = new DogBreadDecorator(new DogDirtyDecorator(dogsArray.GetIterator(), true), DogBread::Alabai);
    WashEmAll(eDirtyAlabai);
    delete eDirtyAlabai;


// Моем всех маленьких собак
    cout << endl << "Wash everyone small dogs using iterator:" << endl;
    Iterator<Dogs*> *eSmall = new DogSizeDecorator(dogsArray.GetIterator(),DogSize::Small);
    WashEmAll(eSmall);
    delete eSmall;
    cout << endl;
// адаптер
    cout << endl << "Wash everyone medium dirty dogs using iterator:" << endl;
    Iterator<Dogs*> *aw = new ConstIteratorAdapter<std::vector<Dogs*>, Dogs*>(&dogsVector);
    Iterator<Dogs*> *amvw =  new DogSizeDecorator(new DogDirtyDecorator(aw, true), DogSize::Medium);
    WashEmAll(amvw);
    delete amvw;
    return 0;
}
