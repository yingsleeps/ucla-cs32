#include "Sequence.h"

Sequence::Sequence()
{
    // creating the sequence with no nodes
    m_size = 0;
    head = nullptr;
    tail = nullptr;
}

Sequence::Sequence(const Sequence& other)
{
    // the case for copy constructing an empty sequence
    if (other.m_size == 0)
    {
        m_size = 0;
        head = nullptr;
        tail = nullptr;
    }
    else
    {
        // creating the head for the new sequence
        head = new Node;
        head->m_value = other.head->m_value;
        head->previous = nullptr;
        head->next = nullptr;
        m_size = 1;
        
        Node* thisTemp = head;  // temproary node for traversing the new sequence
        Node* otherTemp = other.head;  // temproary node for traversing the other sequence
        // loop for visiting each node of the other sequence
        while (otherTemp->next != nullptr)
        {
            // creating a new node to add to the list and copying over the values
            thisTemp->next = new Node;
            thisTemp->next->m_value = otherTemp->next->m_value;
            thisTemp->next->previous = thisTemp;
            thisTemp->next->next = nullptr;
            // incrementing the temproary nodes and size
            thisTemp = thisTemp->next;
            otherTemp = otherTemp->next;
            m_size++;
        }
        // setting the tail
        tail = thisTemp;
    }
}

Sequence& Sequence::operator=(const Sequence &rhs)
{
    // checks that the two are not at the same address
    if (this != &rhs)
    {
        Sequence temp(rhs);  // creates a copy of what needs to be assigned
        swap(temp);  // then swaps it with this
    }
    return *this;
}

Sequence::~Sequence()
{
    // checks that the sequence is not empty
    if (head != nullptr)
    {
        Node* temp = head;  // temproary node for traversing the list
        Node* tempNext;  // for not losing the pointer to the next node after deletion
        // loop for deleting the nodes until the list is empty
        while (temp != nullptr)
        {
            tempNext = temp->next;
            delete temp;
            temp = tempNext;
        }
    }
}

bool Sequence::empty() const
{
    // checks if the sequence is empty
    if (m_size == 0)
    {
        return true;
    }
    return false;
}

int Sequence::size() const
{
    return m_size;
}

int Sequence::insert(int pos, const ItemType &value)
{
    // checks if the index requested is valid
    if (pos < 0 || pos > m_size)
    {
        pos = -1;
        return pos;
    }
    // the case for adding a node to an empty list
    if (m_size == 0)
    {
        // creating a new node and setting its values
        Node* new_dude = new Node;
        new_dude->m_value = value;
        new_dude->next = nullptr;
        new_dude->previous = nullptr;
        // reassigning the head and tail
        head = new_dude;
        tail = new_dude;
        m_size++;
        return pos;
    }
    // the case for adding the node at the beginning of the list
    else if (pos == 0)
    {
        // creating a new node and setting its values
        Node* new_dude = new Node;
        new_dude->m_value = value;
        new_dude->next = head;
        new_dude->previous = nullptr;
        // reassigning the head
        head->previous = new_dude;
        head = new_dude;
        m_size++;
        return pos;
    }
    // the case for adding the node at the end of the list
    else if (pos == m_size)
    {
        // creating a new node and setting its values
        Node* new_dude = new Node;
        new_dude->m_value = value;
        new_dude->next = nullptr;
        new_dude->previous = tail;
        // reassigning the tail
        tail->next = new_dude;
        tail = new_dude;
        m_size++;
        return pos;
    }
    // the case for adding the node somewhere in the middle of the list
    else
    {
        Node* temp = head;  // temproary node for traversing the list
        int index = 0;  // to keep track of the index of each node
        // loop for finding the index needed
        while (index != pos && temp->next != nullptr)
        {
            index++;
            temp = temp->next;
        }
        // creating a new node and setting its values
        Node* new_dude = new Node;
        new_dude->m_value = value;
        new_dude->next = temp;
        new_dude->previous = temp->previous;
        // reassigning the pointers to the new node
        temp->previous->next = new_dude;
        temp->previous = new_dude;
        m_size++;
        return pos;
    }
}

int Sequence::insert(const ItemType &value)
{
    int pos = 0;  // to store the index/position of the nodes
    // case for adding a node to an empty list
    if (m_size == 0)
    {
        // creating a new node and setting its values
        Node* new_dude = new Node;
        new_dude->m_value = value;
        new_dude->next = nullptr;
        new_dude->previous = nullptr;
        // reassigning the head and tail
        head = new_dude;
        tail = new_dude;
        m_size++;
        return pos;
    }
    else
    {
        Node* temp = head;  // temproary node for traversing the list
        // loop for finding position
        while (temp->m_value < value)
        {
            // if at the end of the list, then the new value must go at the end
            if (temp->next == nullptr)
            {
                pos = m_size;
                break;
            }
            pos++;
            temp = temp->next;
        }
        // the case for adding the node at the beginning of the list
        if (pos == 0)
        {
            // creating a new node and setting its values
            Node* new_dude = new Node;
            new_dude->m_value = value;
            new_dude->next = head;
            new_dude->previous = nullptr;
            // reassigning the head
            head->previous = new_dude;
            head = new_dude;
            m_size++;
            return pos;
        }
        // the case for adding the node at the end of the list
        else if (pos == m_size)
        {
            // creating a new node and setting its values
            Node* new_dude = new Node;
            new_dude->m_value = value;
            new_dude->next = nullptr;
            new_dude->previous = tail;
            // reassigning the tail
            tail->next = new_dude;
            tail = new_dude;
            m_size++;
            return pos;
        }
        // the case for adding the node somewhere in the middle of the list
        else
        {
            // creating a new node and setting its values
            Node* new_dude = new Node;
            new_dude->m_value = value;
            new_dude->next = temp;
            new_dude->previous = temp->previous;
            // reassigning the pointers to the new node
            temp->previous->next = new_dude;
            temp->previous = new_dude;
            m_size++;
            return pos;
        }
    }
}

bool Sequence::erase(int pos)
{
    // checks if the index requested is valid
    if (pos >= m_size || pos < 0)
    {
        return false;
    }
    else
    {
        Node* temp = head;  // temproary node for traversing the list
        int index = 0;  // to keep track of the index of each node
        // loop for finding the index requested
        while (index != pos && temp->next != nullptr)
        {
            index++;
            temp = temp->next;
        }
        // the case for one node in the list
        if (m_size == 1)
        {
            head = nullptr;
            tail = nullptr;
            m_size--;
            delete temp;
            return true;
        }
        // the case for erasing the first node in the list
        else if (index == 0)
        {
            head = temp->next;
            temp->next->previous = nullptr;
            m_size--;
            delete temp;
            return true;
        }
        // the case for erasing the last node in the list
        else if (index == (m_size - 1))
        {
            tail = temp->previous;
            temp->previous->next = nullptr;
            m_size--;
            delete temp;
            return true;
        }
        // the case for erasing a node somewhere in the middle of the list
        else
        {
            temp->previous->next = temp->next;
            temp->next->previous = temp->previous;
            m_size--;
            delete temp;
            return true;
        }
    }
}

int Sequence::remove(const ItemType &value)
{
    int removed = 0;  // counter for how many nodes removed
    int index = 0;  // to keep track of the index for each node
    Node* temp = head;  // temproary node for traversing the list
    // for the case of an empty sequence
    if (head == nullptr)
    {
        return removed;
    }
    // for the case of one item in sequence
    if (m_size == 1 && head->m_value == value)
    {
        this->erase(index);
        removed++;
        return removed;
    }
    // loop for finding the nodes with the needed value and erasing accordingly
    while (temp->next != nullptr && index < m_size)
    {
        // if values match, the node is erased
        if (temp->m_value == value)
        {
            temp = temp->next;
            this->erase(index);
            removed++;
        }
        // if not, temp and index are incremented
        else
        {
            index++;
            temp = temp->next;
        }
    }
    // the case for removing a node at the end of the sequence
    if (index == (m_size - 1) && temp->m_value == value)
    {
        this->erase(index);
        removed++;
    }
    return removed;
}

bool Sequence::get(int pos, ItemType &value) const
{
    // checks if the index requested is valid
    if (pos >= m_size || pos < 0)
    {
        return false;
    }
    else
    {
        Node* temp = head;  // temproary node for traversing the list
        int index = 0;  // to keep track of the index for each node
        // loop for finding the node with the needed position
        while (index != pos && temp->next != nullptr)
        {
            index++;
            temp = temp->next;
        }
        value = temp->m_value;  // assigns the value at the found node to value
        return true;
    }
}

bool Sequence::set(int pos, const ItemType &value)
{
    // checks if the index requested is valid
    if (pos >= m_size || pos < 0)
    {
        return false;
    }
    else
    {
        Node* temp = head;  // temproary node for traversing the list
        int index = 0;  // for keeping track of the index of each node
        // loop for finding the node at the needed position
        while (index != pos && temp->next != nullptr)
        {
            index++;
            temp = temp->next;
        }
        temp->m_value = value;  // assigns value to the value at the found node
        return true;
    }
}

int Sequence::find(const ItemType &value) const
{
    // checks if the sequence is empty
    if (head == nullptr)
    {
        return -1;
    }
    int pos = 0;  // to store the index of the node
    Node* temp = head;  // temproary node for traversing the list
    // the case for a one item list
    if (m_size == 1 && head->m_value == value)
    {
        return pos;
    }
    // loop fopr finding the same value in the sequence
    while (temp->next != nullptr)
    {
        // checks if the values match
        if (temp->m_value == value)
        {
            return pos;
        }
        // if not, temp and pos are incremented
        else
        {
            pos++;
            temp = temp->next;
        }
        // the case for finding a node at the end of the sequence
        if (pos == (m_size - 1) && temp->m_value == value)
        {
            return pos;
        }
    }
    // if not found, -1 is returned
    return -1;
}

void Sequence::swap(Sequence &other)
{
    // swaps the sizes
    int temp_size = other.m_size;
    other.m_size = m_size;
    m_size = temp_size;
    
    //swap the heads
    Node* temp_head = other.head;
    other.head = head;
    head = temp_head;
    
    // swap the tails
    Node* temp_tail = other.tail;
    other.tail = tail;
    tail = temp_tail;
    
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
    // checks to see if the sequences passed are valid
    if (seq2.size() == 0 || seq2.size() > seq1.size())
    {
        return -1;
    }
    else
    {
        ItemType firstItem, secondItem;
        int k = 0;
        // loops through seq1
        for (int i = 0; i < seq1.size(); i++)
        {
            seq1.get(i, firstItem);  // accesses the value of node with index i
            // loops through seq2
            while (k < seq2.size())
            {
                seq2.get(k, secondItem);  // accesses the value of node with index k
                // if the two values don't match, keep seq2 at the same index
                if (secondItem != firstItem)
                {
                    k = 0;
                    break;
                }
                // if they do, increment seq2 to check the next value
                k++;
                break;
            }
            // if k reaches the end of seq2, seq2 is a consecutive subsequence of seq1
            if (k == seq2.size())
            {
                return i - (seq2.size() - 1);  // the starting position in seq1
            }
        }
        // if k doesn't reach the end of seq2, seq2 is not a consecutive subsequence of seq1
        return -1;
    }
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    ItemType seq1Item, seq2Item;
    int index1 = 0;
    int index2 = 0;
    
    // checks if seq1 and seq2 are empty
    if (seq1.size() == 0 && seq2.size() == 0)
    {
        // erases whatever was in result before
        for (int i = (result.size() - 1); i >= 0; i--)
        {
            result.erase(i);
        }
    }
    // checks if seq1 is empty, if so result will just be seq2
    else if (seq1.size() == 0)
    {
        result = seq2;
    }
    // checks if seq2 is empty, if so result will just be seq1
    else if (seq2.size() == 0)
    {
        result = seq1;
    }
    else if (&seq1 == &result)
    {
        Sequence seq1copy(seq1);
        // for the case of two same-sized sequences
        if (seq1copy.size() == seq2.size())
        {
            // loop for adding the items from seq1 and seq2
            for (int i = 0; i < (seq1copy.size() + seq2.size()); i++)
            {
                // loop for accessing every item in seq1
                while (index1 < seq1copy.size())
                {
                    seq1copy.get(index1, seq1Item);
                    index1++;
                    break;
                }
                // loop for accessing every item in seq2
                while (index2 < seq2.size())
                {
                    seq2.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert((2*i), seq1Item);  // even indexes, seq1
                result.insert(((2*i) + 1), seq2Item);  // odd indexes, seq2
            }
            // delete the items that result started with
            if (result.size() != (seq1copy.size() + seq2.size()))
            {
                for (int i = (result.size() - 1); i >= (seq1copy.size() + seq2.size()); i--)
                {
                    result.erase(i);
                }
            }
        }
        // for the case of seq1 being smaller
        else if (seq1copy.size() < seq2.size())
        {
            // loop for adding the items from seq1 and seq2
            for (int i = 0; i < seq1copy.size(); i++)
            {
                // loop for accessing every item in seq1
                while (index1 < seq1copy.size())
                {
                    seq1copy.get(index1, seq1Item);
                    index1++;
                    break;
                }
                // loop for accessing every item in seq2
                while (index2 < seq2.size())
                {
                    seq2.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert((2*i), seq1Item);  // even indexes, seq1
                result.insert(((2*i) + 1), seq2Item);  // odd indexes, seq2
            }
            // loop for adding the rest of seq2
            for (int i = (2 * seq1copy.size()); i < (seq1copy.size() + seq2.size()); i++)
            {
                // loop for accessing every item in seq2
                while (index2 < seq2.size())
                {
                    seq2.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert(i, seq2Item);
            }
            // delete the items that result started with
            if (result.size() != (seq1copy.size() + seq2.size()))
            {
                for (int i = (result.size() - 1); i >= (seq1copy.size() + seq2.size()); i--)
                {
                    result.erase(i);
                }
            }
        }
        // for the case of seq2 being smaller
        else if (seq1copy.size() > seq2.size())
        {
            // loop for adding the items from seq1 and seq2
            for (int i = 0; i < seq2.size(); i++)
            {
                // loop for accessing every item in seq1
                while (index1 < seq1copy.size())
                {
                    seq1copy.get(index1, seq1Item);
                    index1++;
                    break;
                }
                // loop for accessing every item in seq2
                while (index2 < seq2.size())
                {
                    seq2.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert((2*i), seq1Item);  // even indexes, seq1
                result.insert(((2*i) + 1), seq2Item);  // odd indexes, seq2
            }
            // loop for adding the rest of seq2
            for (int i = (2 * seq2.size()); i < (seq1copy.size() + seq2.size()); i++)
            {
                // loop for accessing every item in seq2
                while (index1 < seq1copy.size())
                {
                    seq1copy.get(index1, seq1Item);
                    index1++;
                    break;
                }
                result.insert(i, seq1Item);
            }
            // delete the items that result started with
            if (result.size() != (seq1copy.size() + seq2.size()))
            {
                for (int i = (result.size() - 1); i >= (seq1copy.size() + seq2.size()); i--)
                {
                    result.erase(i);
                }
            }
        }
    }
    else if (&seq2 == &result)
    {
        Sequence seq2copy(seq2);
        // for the case of two same-sized sequences
        if (seq1.size() == seq2copy.size())
        {
            // loop for adding the items from seq1 and seq2
            for (int i = 0; i < (seq1.size() + seq2copy.size()); i++)
            {
                // loop for accessing every item in seq1
                while (index1 < seq1.size())
                {
                    seq1.get(index1, seq1Item);
                    index1++;
                    break;
                }
                // loop for accessing every item in seq2
                while (index2 < seq2copy.size())
                {
                    seq2copy.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert((2*i), seq1Item);  // even indexes, seq1
                result.insert(((2*i) + 1), seq2Item);  // odd indexes, seq2
            }
            // delete the items that result started with
            if (result.size() != (seq1.size() + seq2copy.size()))
            {
                for (int i = (result.size() - 1); i >= (seq1.size() + seq2copy.size()); i--)
                {
                    result.erase(i);
                }
            }
        }
        // for the case of seq1 being smaller
        else if (seq1.size() < seq2copy.size())
        {
            // loop for adding the items from seq1 and seq2
            for (int i = 0; i < seq1.size(); i++)
            {
                // loop for accessing every item in seq1
                while (index1 < seq1.size())
                {
                    seq1.get(index1, seq1Item);
                    index1++;
                    break;
                }
                // loop for accessing every item in seq2
                while (index2 < seq2copy.size())
                {
                    seq2copy.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert((2*i), seq1Item);  // even indexes, seq1
                result.insert(((2*i) + 1), seq2Item);  // odd indexes, seq2
            }
            // loop for adding the rest of seq2
            for (int i = (2 * seq1.size()); i < (seq1.size() + seq2copy.size()); i++)
            {
                // loop for accessing every item in seq2
                while (index2 < seq2copy.size())
                {
                    seq2copy.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert(i, seq2Item);
            }
            // delete the items that result started with
            if (result.size() != (seq1.size() + seq2copy.size()))
            {
                for (int i = (result.size() - 1); i >= (seq1.size() + seq2copy.size()); i--)
                {
                    result.erase(i);
                }
            }
        }
        // for the case of seq2 being smaller
        else if (seq1.size() > seq2copy.size())
        {
            // loop for adding the items from seq1 and seq2
            for (int i = 0; i < seq2copy.size(); i++)
            {
                // loop for accessing every item in seq1
                while (index1 < seq1.size())
                {
                    seq1.get(index1, seq1Item);
                    index1++;
                    break;
                }
                // loop for accessing every item in seq2
                while (index2 < seq2copy.size())
                {
                    seq2copy.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert((2*i), seq1Item);  // even indexes, seq1
                result.insert(((2*i) + 1), seq2Item);  // odd indexes, seq2
            }
            // loop for adding the rest of seq2
            for (int i = (2 * seq2copy.size()); i < (seq1.size() + seq2copy.size()); i++)
            {
                // loop for accessing every item in seq2
                while (index1 < seq1.size())
                {
                    seq1.get(index1, seq1Item);
                    index1++;
                    break;
                }
                result.insert(i, seq1Item);
            }
            // delete the items that result started with
            if (result.size() != (seq1.size() + seq2copy.size()))
            {
                for (int i = (result.size() - 1); i >= (seq1.size() + seq2copy.size()); i--)
                {
                    result.erase(i);
                }
            }
        }
    }
    else
    {
        // for the case of two same-sized sequences
        if (seq1.size() == seq2.size())
        {
            // loop for adding the items from seq1 and seq2
            for (int i = 0; i < (seq1.size() + seq2.size()); i++)
            {
                // loop for accessing every item in seq1
                while (index1 < seq1.size())
                {
                    seq1.get(index1, seq1Item);
                    index1++;
                    break;
                }
                // loop for accessing every item in seq2
                while (index2 < seq2.size())
                {
                    seq2.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert((2*i), seq1Item);  // even indexes, seq1
                result.insert(((2*i) + 1), seq2Item);  // odd indexes, seq2
            }
            // delete the items that result started with
            if (result.size() != (seq1.size() + seq2.size()))
            {
                for (int i = (result.size() - 1); i >= (seq1.size() + seq2.size()); i--)
                {
                    result.erase(i);
                }
            }
        }
        // for the case of seq1 being smaller
        else if (seq1.size() < seq2.size())
        {
            // loop for adding the items from seq1 and seq2
            for (int i = 0; i < seq1.size(); i++)
            {
                // loop for accessing every item in seq1
                while (index1 < seq1.size())
                {
                    seq1.get(index1, seq1Item);
                    index1++;
                    break;
                }
                // loop for accessing every item in seq2
                while (index2 < seq2.size())
                {
                    seq2.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert((2*i), seq1Item);  // even indexes, seq1
                result.insert(((2*i) + 1), seq2Item);  // odd indexes, seq2
            }
            // loop for adding the rest of seq2
            for (int i = (2 * seq1.size()); i < (seq1.size() + seq2.size()); i++)
            {
                // loop for accessing every item in seq2
                while (index2 < seq2.size())
                {
                    seq2.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert(i, seq2Item);
            }
            // delete the items that result started with
            if (result.size() != (seq1.size() + seq2.size()))
            {
                for (int i = (result.size() - 1); i >= (seq1.size() + seq2.size()); i--)
                {
                    result.erase(i);
                }
            }
        }
        // for the case of seq2 being smaller
        else if (seq1.size() > seq2.size())
        {
            // loop for adding the items from seq1 and seq2
            for (int i = 0; i < seq2.size(); i++)
            {
                // loop for accessing every item in seq1
                while (index1 < seq1.size())
                {
                    seq1.get(index1, seq1Item);
                    index1++;
                    break;
                }
                // loop for accessing every item in seq2
                while (index2 < seq2.size())
                {
                    seq2.get(index2, seq2Item);
                    index2++;
                    break;
                }
                result.insert((2*i), seq1Item);  // even indexes, seq1
                result.insert(((2*i) + 1), seq2Item);  // odd indexes, seq2
            }
            // loop for adding the rest of seq1
            for (int i = (2 * seq2.size()); i < (seq1.size() + seq2.size()); i++)
            {
                // loop for accessing every item in seq1
                while (index1 < seq1.size())
                {
                    seq1.get(index1, seq1Item);
                    index1++;
                    break;
                }
                result.insert(i, seq1Item);
            }
            // delete the items that result started with
            if (result.size() != (seq1.size() + seq2.size()))
            {
                for (int i = (result.size() - 1); i >= (seq1.size() + seq2.size()); i--)
                {
                    result.erase(i);
                }
            }
        }
    }
}
