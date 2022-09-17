
#include <vector>
#include <algorithm>

#include "triangular.h"
#include "triangular_iterator.h"

using namespace std;

/********************************************************************************************************/
/*************************************************4.2****************************************************/

Triangular::Triangular(int len, int bp) :
    beg_pos_(bp > 1 ? bp : 1), length_(len > 1 ? len : 1)
{
    next_ = beg_pos_ - 1;
    unsigned int elem_cnt = length_ + beg_pos_ + 1;

    /* 4.5 */
    if (elems_.size() < elem_cnt)
        GenElements(elem_cnt);
}

// Triangular::Triangular(int len) : length_(len > 1 ? len : 1)
// {
//     beg_pos_ = 1;
//     next_ = 0;

//     unsigned int elem_cnt = length_ + beg_pos_ + 1;
//     if (elems_.size() < elem_cnt)
//         GenElements(elem_cnt);
// }

Triangular::Triangular(const Triangular &rhs) :
                        beg_pos_(rhs.beg_pos_),
                        length_(rhs.length_),
                        next_(rhs.beg_pos_ - 1)
{ }

/********************************************************************************************************/
/*************************************************4.3****************************************************/

int Sum(const Triangular &item)
{
    if (!item.Length()) return 0;

    int val = 0, sum = 0;
    item.NextReset();
    while (item.Next(val))
        sum += val;

    return sum;
}

bool Triangular::Next(int &val) const
{
    if (next_ < beg_pos_ + length_ - 1)
    {
        val = elems_[next_++];
        return true;
    }

    next_ = 0;
    return false;
}

/********************************************************************************************************/
/*************************************************4.4****************************************************/

Triangular& Triangular::Copy(const Triangular &rhs)
{
    if (this != &rhs)
    {
        beg_pos_ = rhs.beg_pos_;
        length_ = rhs.length_;
        next_ = rhs.beg_pos_ - 1;
    }

    return *this;
}

Triangular& Triangular::operator=(const Triangular &rhs)
{
    if (this != &rhs)
    {
        beg_pos_ = rhs.beg_pos_;
        length_ = rhs.length_;
        next_ = rhs.beg_pos_ - 1;
    }

    return *this;
}

/********************************************************************************************************/
/*************************************************4.5****************************************************/

vector<int> Triangular::elems_;
int         Triangular::init_size_ = 1024;

void Triangular::GenElemsToValue(int value)
{
    int ix = elems_.size();
    if (!ix)
    {
        elems_.push_back(1);
        ix = 1;
    }

    while (elems_[ix - 1] < value && ix < max_size_)
    {
        ++ix;
        elems_.push_back(ix * (ix + 1) / 2);
    }

    if (ix == max_size_)
    {
        cerr << "Triangular sequence: value to large: "
             << value << " --- exceeds max size of "
             << max_size_ << endl;
    }
}

bool Triangular::IsElem(int val)
{
    if (!elems_.size() || elems_[elems_.size() - 1] < val)
        GenElemsToValue(val);

    return find(elems_.begin(), elems_.end(), val) != elems_.end();
}

void Triangular::GenElements(int length)
{
    if (length < 0 || length > max_size_)
    {
        cerr << "Triangular sequence: invalid size: "
             << length << " --- max size is: " << max_size_
             << endl;
        return;
    }

    int ix = elems_.size() ? elems_.size() + 1 : 1;
    for (; ix <= length; ++ix)
        elems_.push_back(ix * (ix + 1) / 2);
}

void Triangular::Display(int len, int bp, ostream &os)
{
    if (len <= 0 || bp <= 0)
    {
        cerr << "invalid parameters, can't handle request: "
             << len << ", " << bp << endl;
        return ;
    }

    unsigned int elems = len + bp - 1;
    if (elems_.size() < elems)
        GenElements(elems);

    for (unsigned int ix = bp - 1; ix < elems; ++ix)
        os << elems_[ix] << ' ';
    os << endl;
}

/********************************************************************************************************/
/*************************************************4.6****************************************************/

Triangular::Iterator Triangular::Begin() const
{
    return Iterator(beg_pos_);
}

Triangular::Iterator Triangular::End() const
{
    return Iterator(beg_pos_ + length_);    // 不需要-1
}

/********************************************************************************************************/
/*************************************************4.7****************************************************/

ostream &operator<<(ostream &os, const Triangular &rhs)
{
    os << "( " << rhs.length_ << ", " << rhs.beg_pos_ << " ) ";
    rhs.Display(rhs.length_, rhs.beg_pos_, os);

    return os;
}

istream& operator>>(istream& in, Triangular& rhs)
{
    char ch1, ch2;
    int bp, len;

    in >> bp >> len;
    rhs.BegPos(bp);
    rhs.Length(len);
    rhs.NextReset();

    return in;
}
