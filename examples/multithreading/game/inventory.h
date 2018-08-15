/*! \file inventory.h
 *
 *  Describes an in-game inventory
 */
#pragma once
#include "item.h"
#include <sadhash.h>

namespace nodes
{
class InventoryNode;
}


namespace game
{

 /*! \class game::Inventory
  *
  *  An inventory for storing each item
  */
class Inventory
{
public:
    /*! An inventory row map
     */
    typedef sad::Hash<int, game::Item *> RowHashMap;
    /*! A list of item as a hash map
     */
    typedef sad::Hash<int, game::Inventory::RowHashMap> HashMap;
    /*! Creates an empty directory
     */
    Inventory();
    /*! Frees all items, deleting them
     */
    virtual ~Inventory();
    /*! Clears all inventory items in inventory
     */
    void clear();
    /*! Returns item by it's index in inventory
     *  \param[in] i row
     *  \param[in] j column
     *  \return item or NULL if not found
     */
    game::Item* getItemByIndex(int i, int j);
    /*! Adds item to inventory
     *  \param[in] item an item in inventory
     *  \return false if inventory is full, true otherwise
     */
    bool addItem(game::Item* item);
    /*! Replaces items by positions
     *  \param[in] i1 row for first item
     *  \param[in] j1 column for first item
     *  \param[in] i2 row for second item
     *  \param[in] j2 column for second item
     *  \return false if invalid indexes specified
     */
    bool replaceItem(int i1, int j1, int i2, int j2);
    /*! Tries to store item into inventory
     *  \param[in] i a row index
     *  \param[in] j a column index
     *  \param[in] item an item
     *  \return false if place already taken
     */
    bool storeItem(int i, int j, game::Item* item);
    /*! Takes item, returning it but removing also an item
     *  \param[in] i an index
     *  \param[in] j an index
     *  \return pointer to item or NULL if not found
     */
    game::Item* takeItem(int i, int j);
    /*! Returns an items from inventory
     *  \return an items list from inventory
     */
    const game::Inventory::HashMap& items() const;
    /*! Sets node for inventory
     *  \param[in] node a node item data
     */
    void setNode(nodes::InventoryNode* node);
    /*! Returns node for inventory
     */
    nodes::InventoryNode* node() const;
    /*! A maximal horizontal cells of inventory
     */
    static const int Width;
    /*! A maximal vertical cells of inventory
     */
    static const int Height;
protected:
    /*! A hash-map, storing all of items
     */
    game::Inventory::HashMap m_items;
    /*! Amount of items in inventory
     */
    size_t m_items_count;
    /*! An inventory node
     */
    nodes::InventoryNode* m_node;
private:
    /*! A copying is disabled for inventory
     *  \param[in] inventory other inventory
     */
    Inventory(const game::Inventory& inventory);
    /*! A copying is disabled for inventory
    *  \param[in] inventory other inventory
    *  \return result
    */
    game::Inventory& operator=(const game::Inventory& inventory);
};

}
