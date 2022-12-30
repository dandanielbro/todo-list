#include "todomodel.h"

#include "todolist.h"

ToDoModel::ToDoModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_list(nullptr)
{
}

int ToDoModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !m_list)
        return 0;

    return m_list->items().size();
}

QVariant ToDoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // Return the corresponding data on the specified row by index
    const ToDoItem item = m_list->items().at(index.row());
    switch (role) {
    case DoneRole:
        return QVariant(item.done);
    case DescriptionRole:
        return QVariant(item.description);
    }
    return QVariant();
}

bool ToDoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // Return with false when m_list is a null pointer
    if (!m_list)
        return false;

    ToDoItem item = m_list->items().at(index.row());
    switch (role) {
    case DoneRole:
        item.done = value.toBool(); // Convert QVariant to correct type for the assigning
        break;
    case DescriptionRole:
        item.description = value.toString(); // Convert QVariant to correct type for the assigning
        break;
    }

    if (m_list->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ToDoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ToDoModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[DoneRole] = "done";
    names[DescriptionRole] = "description";
    return names;
}

ToDoList *ToDoModel::list() const
{
    return m_list;
}

void ToDoModel::setList(ToDoList *list)
{
    beginResetModel();

    if (m_list)
        m_list->disconnect(this);

    m_list = list;

    if (m_list) {
        connect(m_list, &ToDoList::preItemAppended, this, [=]() {
            const int index = m_list->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(m_list, &ToDoList::postItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(m_list, &ToDoList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(m_list, &ToDoList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });
    }

    endResetModel();
}
