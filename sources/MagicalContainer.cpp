#include "MagicalContainer.hpp"
#include <cassert>
#include <algorithm>
using namespace std;
namespace ariel
{
    bool MagicalContainer::isPrime(int num)
    {
        if (num < 2)
        {
            return false;
        }
        // no need to run more than num/2 as this is the biggest divder
        for (int i = 2; i <= num / 2; i++)
        {
            if (num % i == 0)
                return false;
        }
        return true;
    }
    // this methods calls each container and updates it
    void MagicalContainer::updateIterContainer(int *elementPtr)
    {

        updateAscContainer(elementPtr);
        updateCrossContainer(); // using the updated asc_container so elmPtr is already in it
        updatePrimeContainer(elementPtr);
    }
    void MagicalContainer::updatePrimeContainer(int *elementPtr)
    {
        if (isPrime(*elementPtr))
        {
            auto it = lower_bound(prime_container.begin(), prime_container.end(), elementPtr,
                                  [](int *ptr1, int *ptr2)
                                  { return *ptr1 < *ptr2; });
            prime_container.insert(it, elementPtr);
        }
    }
    void MagicalContainer::updateAscContainer(int *elementPtr)
    {
        auto it = lower_bound(asc_container.begin(), asc_container.end(), elementPtr,
                              [](int *ptr1, int *ptr2)
                              { return *ptr1 < *ptr2; });
        asc_container.insert(it, elementPtr);
    }
    void MagicalContainer::updateCrossContainer()
    {
        // remove and add are the same as we are using asc_container
        //  we need to clear the vector and use the pointers from asc_container to reinsert them in cross order
        size_t size = asc_container.size();

        cross_container.clear();
        for (size_t i = 0; i < size; i++)
        {
            size_t inserted = i;
            size_t j = size - i - 1;
            if (inserted < j)
            {
                cross_container.push_back(asc_container[inserted]);
                cross_container.push_back(asc_container[j]);
            }
            else if (inserted == j)
            {
                cross_container.push_back(asc_container[inserted]);
            }
            else
            {
                break;
            }
        }
    }

    // magical container
    void MagicalContainer::addElement(int element)
    { //? assuming only one element of each value can be in the container

        int *elementPtr = nullptr;

        if (elements_container.empty())
        {
            elements_container.push_back(element);
            elementPtr = &elements_container.back();
        }
        else
        {
            auto it = std::lower_bound(elements_container.begin(), elements_container.end(), element);
            it = elements_container.insert(it, element);
            elementPtr = &(*it);
        }

        updateIterContainer(elementPtr);
    }
    void MagicalContainer::removeElement(int element)
    {
        // first check if element is in container
        auto it = find(elements_container.begin(), elements_container.end(), element);
        if (it == elements_container.end())
        {
            throw runtime_error("element not in container");
        }
        // remove from container
        // erase from the iter containers
        auto it2 = find(asc_container.begin(), asc_container.end(), &(*it));
        asc_container.erase(it2);
        if (isPrime(element))
        {
            auto it3 = find(prime_container.begin(), prime_container.end(), &(*it));
            prime_container.erase(it3);
        }
        // update cross container
        elements_container.erase(it);
        updateCrossContainer();
    }
    size_t MagicalContainer::size() const
    {
        return elements_container.size();
    }

    //* AscendingIterator
    MagicalContainer::AscendingIterator::AscendingIterator(MagicalContainer &container) : container(container), index(0)
    {
    }
    MagicalContainer::AscendingIterator::AscendingIterator(MagicalContainer &container, size_t index) : container(container), index(index)
    { // for end()
    }

    MagicalContainer::AscendingIterator::AscendingIterator(const AscendingIterator &other) : container(other.container), index(other.index)
    {
    }

    MagicalContainer::AscendingIterator::~AscendingIterator()
    {
    }
    MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator=(const AscendingIterator &other)
    {
        if (&container != &other.container)
        {
            throw runtime_error("cant assign iterators of different containers");
        }
        if (this != &other)
        {
            index = other.index;
        }
        return *this;
    }
    MagicalContainer::AscendingIterator::AscendingIterator(AscendingIterator &&other) noexcept : container(other.container), index(other.index)
    {
    }
    MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator=(AscendingIterator &&other) noexcept
    {
        if (this != &other)
        {
            container = other.container;
            index = other.index;
        }
        return *this;
    }

    int MagicalContainer::AscendingIterator::operator*() const
    {
        if (index >= container.asc_container.size())
        {
            throw out_of_range("out of range");
        }
        return *(container.asc_container[index]);
    }
    MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator++()
    {
        if (index >= container.asc_container.size()) // if index> size we already out of range
        {
            throw runtime_error("cant increment beyond bounds");
        }
        index++;
        return *this;
    }
    bool MagicalContainer::AscendingIterator::operator<(const CustomIterator &other) const
    {
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const AscendingIterator &asc_iter = static_cast<const AscendingIterator &>(other);
        // check if they have the same container
        if (&container != &asc_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index < asc_iter.index;
    }
    bool MagicalContainer::AscendingIterator::operator>(const CustomIterator &other) const
    {
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const AscendingIterator &asc_iter = static_cast<const AscendingIterator &>(other);
        // check if they have the same container
        if (&container != &asc_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index > asc_iter.index;
    }
    bool MagicalContainer::AscendingIterator::operator==(const CustomIterator &other) const
    { // check if they are the same type, if so check if they have the same container
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const AscendingIterator &asc_iter = static_cast<const AscendingIterator &>(other);
        // check if they have the same container
        if (&container != &asc_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index == asc_iter.index;
    }
    bool MagicalContainer::AscendingIterator::operator!=(const CustomIterator &other) const
    { // check if they are the same type, if so check if they have the same container
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const AscendingIterator &asc_iter = static_cast<const AscendingIterator &>(other);
        // check if they have the same container
        if (&container != &asc_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index != asc_iter.index;
    }
    //* operators with ascending iterator as other
    bool MagicalContainer::AscendingIterator::operator<(const AscendingIterator &other) const
    { // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index < other.index;
    }
    bool MagicalContainer::AscendingIterator::operator>(const AscendingIterator &other) const
    {
        // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index > other.index;
    }
    bool MagicalContainer::AscendingIterator::operator==(const AscendingIterator &other) const
    {
        // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index == other.index;
    }
    bool MagicalContainer::AscendingIterator::operator!=(const AscendingIterator &other) const
    { // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index != other.index;
    }

    //* SideCrossIterator
    MagicalContainer::SideCrossIterator::SideCrossIterator(MagicalContainer &container) : container(container), index(0)
    {
    }
    MagicalContainer::SideCrossIterator::SideCrossIterator(MagicalContainer &container, size_t index) : container(container), index(index)
    {
    }
    MagicalContainer::SideCrossIterator::SideCrossIterator(const SideCrossIterator &other) : container(other.container), index(other.index)
    {
    }
    MagicalContainer::SideCrossIterator::~SideCrossIterator()
    {
    }
    MagicalContainer::SideCrossIterator::SideCrossIterator(SideCrossIterator &&other) noexcept : container(other.container), index(other.index)
    {
    }
    MagicalContainer::SideCrossIterator &MagicalContainer::SideCrossIterator::operator=(SideCrossIterator &&other) noexcept
    {
        if (this != &other)
        {
            container = other.container;
            index = other.index;
        }
        return *this;
    }
    MagicalContainer::SideCrossIterator &MagicalContainer::SideCrossIterator::operator=(const SideCrossIterator &other)
    {
        if (&container != &other.container)
        {
            throw runtime_error("cant assign iterators of different containers");
        }
        if (this != &other)
        {
            index = other.index;
        }
        return *this;
    }
    int MagicalContainer::SideCrossIterator::operator*() const
    {
        if (index >= container.cross_container.size())
        {
            throw out_of_range("out of range");
        }
        return *(container.cross_container[index]);
    }
    MagicalContainer::SideCrossIterator &MagicalContainer::SideCrossIterator::operator++()
    {
        if (index >= container.cross_container.size())
        {
            throw runtime_error("cant increment beyond bounds");
        }
        index++;
        return *this;
    }
    bool MagicalContainer::SideCrossIterator::operator<(const CustomIterator &other) const
    {
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const SideCrossIterator &side_iter = static_cast<const SideCrossIterator &>(other);
        // check if they have the same container
        if (&container != &side_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index < side_iter.index;
    }
    bool MagicalContainer::SideCrossIterator::operator>(const CustomIterator &other) const
    {
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const SideCrossIterator &side_iter = static_cast<const SideCrossIterator &>(other);
        // check if they have the same container
        if (&container != &side_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index > side_iter.index;
    }

    bool MagicalContainer::SideCrossIterator::operator==(const CustomIterator &other) const
    {
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const SideCrossIterator &side_iter = static_cast<const SideCrossIterator &>(other);
        // check if they have the same container
        if (&container != &side_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index == side_iter.index;
    }
    bool MagicalContainer::SideCrossIterator::operator!=(const CustomIterator &other) const
    {
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const SideCrossIterator &side_iter = static_cast<const SideCrossIterator &>(other);
        // check if they have the same container
        if (&container != &side_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index != side_iter.index;
    }
    // operators with side cross iterator as other
    bool MagicalContainer::SideCrossIterator::operator<(const SideCrossIterator &other) const
    {
        // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index < other.index;
    }
    bool MagicalContainer::SideCrossIterator::operator>(const SideCrossIterator &other) const
    {
        // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index > other.index;
    }
    bool MagicalContainer::SideCrossIterator::operator==(const SideCrossIterator &other) const
    {
        // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index == other.index;
    }
    bool MagicalContainer::SideCrossIterator::operator!=(const SideCrossIterator &other) const
    {
        // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index != other.index;
    }

    //* PrimeIterator
    MagicalContainer::PrimeIterator::PrimeIterator(MagicalContainer &container) : container(container), index(0)
    {
    }
    MagicalContainer::PrimeIterator::PrimeIterator(MagicalContainer &container, size_t index) : container(container), index(index)
    {
    }
    MagicalContainer::PrimeIterator::PrimeIterator(const PrimeIterator &other) : container(other.container), index(other.index)
    {
    }
    MagicalContainer::PrimeIterator::~PrimeIterator()
    {
    }
    MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator=(const PrimeIterator &other)
    {
        if (&container != &other.container)
        {
            throw runtime_error("cant assign iterators of different containers");
        }
        if (this != &other)
        {
            index = other.index;
        }
        return *this;
    }
    MagicalContainer::PrimeIterator::PrimeIterator(PrimeIterator &&other) noexcept : container(other.container), index(other.index)
    {
    }
    MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator=(PrimeIterator &&other) noexcept
    {
        if (this != &other)
        {
            container = other.container;
            index = other.index;
        }
        return *this;
    }
    int MagicalContainer::PrimeIterator::operator*() const
    {
        if (index >= container.prime_container.size())
        {
            throw out_of_range("out of range");
        }
        return *(container.prime_container[index]);
    }
    MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator++()
    {
        if (index >= container.prime_container.size())
        {
            throw runtime_error("cant increment beyond bounds");
        }
        index++;
        return *this;
    }
    bool MagicalContainer::PrimeIterator::operator<(const CustomIterator &other) const
    {
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const PrimeIterator &prime_iter = static_cast<const PrimeIterator &>(other);
        // check if they have the same container
        if (&container != &prime_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index < prime_iter.index;
    }
    bool MagicalContainer::PrimeIterator::operator>(const CustomIterator &other) const
    {
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const PrimeIterator &prime_iter = static_cast<const PrimeIterator &>(other);
        // check if they have the same container
        if (&container != &prime_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index > prime_iter.index;
    }
    bool MagicalContainer::PrimeIterator::operator==(const CustomIterator &other) const
    {
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const PrimeIterator &prime_iter = static_cast<const PrimeIterator &>(other);
        // check if they have the same container
        if (&container != &prime_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index == prime_iter.index;
    }
    bool MagicalContainer::PrimeIterator::operator!=(const CustomIterator &other) const
    {
        if (typeid(*this) != typeid(other))
        {
            throw runtime_error("cant compare iterators of different types");
        }
        // downcasting to be able to compare the members of the other object
        const PrimeIterator &prime_iter = static_cast<const PrimeIterator &>(other);
        // check if they have the same container
        if (&container != &prime_iter.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index != prime_iter.index;
    }
    // operators with prime iterator as other
    bool MagicalContainer::PrimeIterator::operator<(const PrimeIterator &other) const
    {
        // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index < other.index;
    }
    bool MagicalContainer::PrimeIterator::operator>(const PrimeIterator &other) const
    {
        // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index > other.index;
    }
    bool MagicalContainer::PrimeIterator::operator==(const PrimeIterator &other) const
    {
        // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index == other.index;
    }
    bool MagicalContainer::PrimeIterator::operator!=(const PrimeIterator &other) const
    {
        // check if they have the same container
        if (&container != &other.container)
        {
            throw runtime_error("cant compare iterators of different containers");
        }
        return index != other.index;
    }

    // *iterator functions
    MagicalContainer::AscendingIterator MagicalContainer::AscendingIterator::begin()
    {
        return AscendingIterator(container);
    }
    MagicalContainer::AscendingIterator MagicalContainer::AscendingIterator::end()
    {
        return AscendingIterator(container, container.asc_container.size());
    }
    MagicalContainer::SideCrossIterator MagicalContainer::SideCrossIterator::begin()
    {
        return SideCrossIterator(container);
    }
    MagicalContainer::SideCrossIterator MagicalContainer::SideCrossIterator::end()
    {
        return SideCrossIterator(container, container.cross_container.size());
    }
    MagicalContainer::PrimeIterator MagicalContainer::PrimeIterator::begin()
    {
        return PrimeIterator(container);
    }
    MagicalContainer::PrimeIterator MagicalContainer::PrimeIterator::end()
    {
        return PrimeIterator(container, container.prime_container.size());
    }
}
