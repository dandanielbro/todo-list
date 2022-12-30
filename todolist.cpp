#include "todolist.h"

ToDoList::ToDoList(QObject *parent) : QObject(parent)
{
    m_Items.append({true, QStringLiteral("Wash the car")});
    m_Items.append({false, QStringLiteral("Fix the bugs")});
    m_Items.append({true, QStringLiteral("Sleep on time")});

}

QVector<ToDoItem> ToDoList::items() const
{
    return m_Items;
}

bool ToDoList::setItemAt(int index, const ToDoItem &item)
{
    // Given index is outside of the list
    if (index < 0 || index >= m_Items.size())
        return false;

    // Compare the given item with the original one
    const ToDoItem &oldItem = m_Items.at(index);

    // When old one and new one is totally the same, return false
    if (item.done == oldItem.done && item.description == oldItem.description)
        return false;

    m_Items[index] = item;
    return true;
}

void ToDoList::appendItem()
{
    emit preItemAppended();

    ToDoItem item;
    item.done = false;
    m_Items.append(item);

    emit postItemAppended();
}

void ToDoList::removeCompletedItems()
{
    for (int i = 0; i < m_Items.size(); ) {
        if (m_Items.at(i).done) {
            emit preItemRemoved(i);

            m_Items.removeAt(i);

            emit postItemRemoved();
        } else {
            ++i;
        }
    }

}
