/***************************************************************************
 * Copyright (C) 2009 by Gopala Krishna A <krishna.ggk@gmail.com>          *
 *                                                                         *
 * This is free software; you can redistribute it and/or modify            *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2, or (at your option)     *
 * any later version.                                                      *
 *                                                                         *
 * This software is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this package; see the file COPYING.  If not, write to        *
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,   *
 * Boston, MA 02110-1301, USA.                                             *
 ***************************************************************************/

#include "associationspacemanager.h"
#include "newassociationwidget.h"
#include "newlinepath.h"
#include "umlwidget.h"

RegionPair::RegionPair(Uml::Region f, Uml::Region s) : first(f), second(s)
{
}

bool RegionPair::isValid() const
{
    return first != Uml::reg_Error;
}

bool RegionPair::operator<(const RegionPair& rhs) const
{
    return id() < rhs.id();
}

int RegionPair::id() const
{
    return (100*first) + second;
}

/**
 * Constructs a new space manager object for given widget.
 */
AssociationSpaceManager::AssociationSpaceManager(UMLWidget *widget)
{
    m_umlWidget = widget;
    Q_ASSERT(widget);
}

/**
 * This method is used to register the AssociationWidget associatied with this
 * UMLWidget along specified region passed.
 *
 * @param  assoc  The AssociationWidget to be registered.
 * @param regions The regions with which the AssociationWidget has to be
 *                registered.
 *
 * @note This method does not call arrange(region) as that is the decision to
 *       be taken dynamically.
 * @note Refer @ref RegionPair to understand why pair is used.
 */
void AssociationSpaceManager::add(New::AssociationWidget *assoc,
        RegionPair regions)
{
    if (!regions.isValid()) return;

    if (isRegistered(assoc)) {
        uDebug() << assoc->name() << " is already registered!";
        return;
    }

    m_regionsAssociationsMap[regions] << assoc;
    m_registeredAssociationSet << assoc;
}

/**
 * This method unregisters the AssociationWidget by removing it from regions
 * specific list.
 *
 * @return The last regions occupied by AssociationWidget.
 *
 * @note The AssociationWidget is however @b not deleted.
 * @note Also the arrange method is not called.
 * @note Refer @ref RegionPair to understand why pair is used.
 */
RegionPair AssociationSpaceManager::remove(New::AssociationWidget *assoc)
{
    if (!isRegistered(assoc)) {
        uDebug() << assoc->name() << " is not registered!";
        return Uml::reg_Error;
    }

    RegionPair reg = regions(assoc);
    //TODO: Remove these checks after extensive testing.
    Q_ASSERT(reg.isValid());

    m_regionsAssociationsMap[reg].removeOne(assoc);
    m_registeredAssociationSet.remove(assoc);

    return reg;
}

/**
 * This method returns the point for given \a assoc which acts as reference
 * for arranging the association widget lines of particular region.
 *
 * The reference point is either the penultimate point or other widget's center
 * based on whether number of points is greater than two or equal to two
 * respectively.
 */
QPointF AssociationSpaceManager::referencePoint(New::AssociationWidget *assoc) const
{
    UMLWidget *widA = assoc->widgetForRole(Uml::A);
    UMLWidget *widB = assoc->widgetForRole(Uml::B);
    QPointF retVal;
    const int pointCount = assoc->associationLine()->count();
    Q_ASSERT(pointCount >= 2);
    if (pointCount == 2) {
        if (widA == m_umlWidget) {
            retVal = widB->sceneRect().center();
        } else if (widB == m_umlWidget) {
            retVal = widA->sceneRect().center();
        } else {
            uWarning() << "Passed association " << assoc->name()
                       << " is not managed by this AssociationSpaceManager";
        }
    } else {
        if (widA == m_umlWidget) {
            retVal = assoc->mapToScene(assoc->associationLine()->point(1));
        } else if (widB == m_umlWidget) {
            retVal = assoc->mapToScene(assoc->associationLine()->point(pointCount-2));
        } else {
            uWarning() << "Passed association " << assoc->name()
                       << " is not managed by this AssociationSpaceManager";
        }
    }
    return retVal;
}

/**
 * This method arranges the AssociationWidget line end points for given
 * regions based on its x or y value of the reference point depending upon
 * the region.
 *
 * @see AssociationSpaceManager::referencePoint
 * @note Refer @ref RegionPair to understand why pair is used.
 */
void AssociationSpaceManager::arrange(RegionPair regions)
{
    if (!regions.isValid()) return;

    QRectF rect = m_umlWidget->sceneRect();
    QList<New::AssociationWidget*> &listRef = m_regionsAssociationsMap[regions];
    if (listRef.isEmpty()) {
        return; // nothing to arrange.
    }

    // Holds whether arrangement is based on x(horizontal) or not (which means
    // its vertically arranged based on y).
    bool xBasis = (regions.first == Uml::reg_North ||
            regions.first == Uml::reg_South);
    // assocDistances contains a list of pairs of New::AssociationWidget and
    // its x or y value depending on region.
    QList<QPair<New::AssociationWidget*, qreal> > assocDistances;
    // This for loop computes the pair values and inserts them in sorted
    // manner based on pair.second variable.
    foreach (New::AssociationWidget* assoc, listRef) {
        // Obtain reference point first.
        QPointF lineStart = referencePoint(assoc);
        // Get x or y coord based on xBasis variable.
        qreal distance = (xBasis ? lineStart.x() : lineStart.y());
        int i = 0;
        // Find appropriate position to insert this new value in.
        while (i < assocDistances.size() && assocDistances[i].second < distance) {
            ++i;
        }
        assocDistances.insert(i, qMakePair(assoc, distance));
    }

    // Now order the New::AssociationWidget in listRef as per ordering in
    // assocDistances list, which is sorted based on x or y value.
    listRef.clear();
    QListIterator<QPair<New::AssociationWidget*, qreal> > it(assocDistances);
    while (it.hasNext()) {
        listRef.append(it.next().first);
    }

    // Do the actual distribution now.
    const qreal totalSpace = xBasis ? rect.width() : rect.height();
    const qreal slotSize = totalSpace / listRef.size();
    qreal pos = (.5 * slotSize) + (xBasis ? rect.left() : rect.top());
    foreach (New::AssociationWidget *assoc, listRef) {
        QPointF end(pos, pos);
        switch (regions.first) {
            case Uml::reg_North: end.setY(rect.top()); break;
            case Uml::reg_East: end.setX(rect.right()); break;
            case Uml::reg_South: end.setY(rect.bottom()); break;
            case Uml::reg_West: end.setX(rect.left()); break;

            case Uml::reg_NorthWest: end = rect.topLeft(); break;
            case Uml::reg_NorthEast: end = rect.topRight(); break;
            case Uml::reg_SouthEast: end = rect.bottomRight(); break;
            case Uml::reg_SouthWest: end = rect.bottomLeft(); break;

            default: break;
        }
        end = assoc->mapFromScene(end);
        New::AssociationLine *line = assoc->associationLine();
        int endIndex = (assoc->roleForWidget(m_umlWidget) == Uml::A ?
            0 : line->count()-1);
        line->setPoint(endIndex, end);
        pos += slotSize;
    }
}

/**
 * @return The RegionPair where assoc's end points resides.
 * @note Refer @ref RegionPair to understand why pair is used.
 */
RegionPair AssociationSpaceManager::regions(New::AssociationWidget *assoc) const
{
    if (!isRegistered(assoc)) {
        return Uml::reg_Error;
    }
    QMapIterator<RegionPair, QList<New::AssociationWidget*> >
        it(m_regionsAssociationsMap);
    while (it.hasNext()) {
        it.next();
        if (it.value().contains(assoc)) {
            return it.key();
        }
    }
    return RegionPair();
}

/**
 * @return Registration status of assoc.
 */
bool AssociationSpaceManager::isRegistered(New::AssociationWidget* assoc) const
{
    return m_registeredAssociationSet.contains(assoc);
}

/**
 * @return The set containing all the New::AssociationWidget managed by this
 *         AssociationSpaceManager.
 */
QSet<New::AssociationWidget*> AssociationSpaceManager::associationWidgets() const
{
    return m_registeredAssociationSet;
}

#include "associationspacemanager.moc"

