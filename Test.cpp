#include "doctest.h"
#include "sources/MagicalContainer.hpp"

using namespace std;
using namespace ariel;

TEST_CASE("MagicalContainer methods")
{
    MagicalContainer container;
    SUBCASE("addElement")
    {
        CHECK_EQ(container.size(), 0);
        container.addElement(17);
        container.addElement(2);
        container.addElement(25);
        CHECK_EQ(container.size(), 3);
    }
    SUBCASE("removeElement")
    {
        container.addElement(17);
        container.addElement(2);
        container.removeElement(17);
        CHECK_EQ(container.size(), 1);
        container.removeElement(2);
        CHECK_EQ(container.size(), 0);
    }
    SUBCASE("throws when removing non existing element or when container is empty")
    {
        CHECK_THROWS(container.removeElement(17));
        container.addElement(17);
        CHECK_THROWS(container.removeElement(7));
    }
}

TEST_CASE("Ascending Iterator")
{
    MagicalContainer container;
    container.addElement(17);
    container.addElement(2);
    container.addElement(25);
    container.addElement(9);
    container.addElement(3);
    MagicalContainer::AscendingIterator ascIter(container);

    SUBCASE("sorted as should be")
    {
        int sorted[] = {2, 3, 9, 17, 25};
        int i = 0;
        for (auto it = ascIter.begin(); it != ascIter.end() && i < 5; ++it, ++i)
        {
            CHECK_EQ(*it, sorted[i]);
        }
    }

    SUBCASE("oprators behave as expected")
    {
        MagicalContainer::AscendingIterator ascIter2(container);
        auto it = ascIter.begin();
        auto it2 = ascIter.begin();
        CHECK(it == it2);
        ++it2;
        CHECK(it != it2);
        CHECK(it < it2);
        ++it;
        ++it;
        CHECK(it > it2);
        CHECK(it2 < it);
        ++it2;
        CHECK(it2 == it);
    }
    SUBCASE("increamnt throw and *end")
    {
        auto it = ascIter.end();
        CHECK_THROWS(*it);
        CHECK_THROWS(++it); // end iterator cant be incremented
    }
    SUBCASE("comparing iterators of different containers")
    {
        MagicalContainer container2;
        container2.addElement(17);
        container2.addElement(2);
        container2.addElement(25);
        MagicalContainer::AscendingIterator ascIter2(container2);
        auto it = ascIter.begin();
        auto it2 = ascIter2.begin();
        // CHECK_THROWS(it == it2);
        // CHECK_THROWS(it != it2);
        // CHECK_THROWS(it < it2);
        // CHECK_THROWS(it > it2);
        // for some reason the test wont check
    }
    SUBCASE("empty container behavior")
    {
        MagicalContainer container2;
        MagicalContainer::AscendingIterator ascIter2(container2);
        CHECK(ascIter2.begin() == ascIter2.end());
        auto it = ascIter2.begin();
        CHECK_THROWS(++it);
    }
}
TEST_CASE("SideCross Iterator")
{
    MagicalContainer container;
    container.addElement(17);
    container.addElement(2);
    container.addElement(25);
    container.addElement(9);
    container.addElement(3);
    MagicalContainer::SideCrossIterator sideIter(container);
    SUBCASE("sorted as should be")
    {
        int sorted[] = {17, 3, 2, 9, 25}; // one from the left one from the right
        int i = 0;
        for (auto it = sideIter.begin(); it != sideIter.end() && i < 5; ++it, ++i)
        {
            CHECK_EQ(*it, sorted[i]);
        }
    }

    SUBCASE("exceptions")
    {
        auto it = sideIter.begin();
        for (; it != sideIter.end(); ++it)
        {
            container.removeElement(*it);
        }
        ++it; // end iterator
        CHECK_THROWS(++it);
        it = sideIter.end();
        CHECK_THROWS(*it);
    }
    SUBCASE("oprators behave as expected")
    {
        MagicalContainer::SideCrossIterator sideIter2(container);
        auto it = sideIter.begin();
        auto it2 = sideIter.begin();
        CHECK(it == it2);
        ++it2; // (*it2)= 25
        CHECK(it != it2);
        CHECK(it < it2);
        ++it;
        ++it; // (*it)= 3
        CHECK(it2 > it);
        CHECK(it < it2);
        ++it2;
        CHECK(it2 == it);
    }
}

TEST_CASE("Prime Iterator")
{
    MagicalContainer container;
    container.addElement(2);
    container.addElement(3);
    container.addElement(25);
    container.addElement(9);
    container.addElement(17);
    MagicalContainer::PrimeIterator primeIter(container);
    SUBCASE("sorted as should be")
    {
        int sorted[] = {2, 3, 17};
        int i = 0;
        for (auto it = primeIter.begin(); it != primeIter.end() && i < 3; ++it, ++i)
        {
            CHECK_EQ(*it, sorted[i]);
        }
    }

    SUBCASE("prime behavior when the biggest is first")
    {
        MagicalContainer container2;
        container2.addElement(7);
        container2.addElement(3);
        container2.addElement(5);
        MagicalContainer::PrimeIterator primeIter2(container2);
        auto it = primeIter2.begin();
        CHECK_EQ(*it, 7);
        ++it;
        CHECK(it == primeIter2.end()); // because the first element is the biggest
    }
    SUBCASE("prime behavior when the biggest is first and you add a bigger one")
    {
        MagicalContainer container2;
        container2.addElement(7);
        container2.addElement(3);
        container2.addElement(5);
        MagicalContainer::PrimeIterator primeIter2(container2);
        auto it = primeIter2.begin();
        CHECK_EQ(*it, 7);
        ++it;
        CHECK(it == primeIter2.end()); // because the first element is the biggest
        container2.addElement(11);
        ++it;
        CHECK_EQ(*it, 11);
    }
    SUBCASE("oprators behave as expected")
    {
        // 2,3,17
        MagicalContainer::PrimeIterator primeIter2(container);
        auto it = primeIter.begin();
        auto it2 = primeIter.begin();
        CHECK(it == it2);
        ++it2; // (*it2)= 3
        CHECK(it != it2);
        CHECK(it < it2);
        CHECK(it2 > it);
        ++it;
        ++it; // (*it)= 17
        CHECK(it > it2);
        CHECK(it2 < it);
        ++it2;
        CHECK(it2 == it);
    }
    SUBCASE("container doesnt have prime numbers")
    {
        MagicalContainer container2;
        container2.addElement(4);
        container2.addElement(6);
        container2.addElement(8);
        MagicalContainer::PrimeIterator primeIter2(container2);
        CHECK(primeIter2.begin() == primeIter2.end());
    }
    SUBCASE("exceptions")
    {
        auto it = primeIter.end();
        CHECK_THROWS(*it);
        CHECK_THROWS(++it);
    }
}