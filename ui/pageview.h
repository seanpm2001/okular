/***************************************************************************
 *   Copyright (C) 2004 by Enrico Ros <eros.kde@email.it>                  *
 *   Copyright (C) 2004 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   With portions of code from kpdf/kpdf_pagewidget.h by:                 *
 *     Copyright (C) 2002 by Wilco Greven <greven@kde.org>                 *
 *     Copyright (C) 2003 by Christophe Devriese                           *
 *                           <Christophe.Devriese@student.kuleuven.ac.be>  *
 *     Copyright (C) 2003 by Laurent Montel <montel@kde.org>               *
 *     Copyright (C) 2003 by Kurt Pfeifle <kpfeifle@danka.de>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
// This file follows coding style described in kdebase/kicker/HACKING

#ifndef _OKULAR_PAGEVIEW_H_
#define _OKULAR_PAGEVIEW_H_

#include <qabstractscrollarea.h>
#include <qlist.h>
#include <qvector.h>
#include "ui/pageviewutils.h"
#include "core/area.h"
#include "core/observer.h"
#include "core/view.h"
#include "core/textpage.h"

class KAction;
class KActionCollection;
class KMenu;
class KUrl;

namespace Okular {
class Action;
class Document;
class Annotation;
class FormField;
}

class FormWidgetIface;
class PageViewPrivate;

/**
 * @short The main view. Handles zoom and continuous mode.. oh, and page
 * @short display of course :-)
 * ...
 */
class PageView : public QAbstractScrollArea, public Okular::DocumentObserver, public Okular::View
{
Q_OBJECT

    public:
        PageView( QWidget *parent, Okular::Document *document );
        ~PageView();

        // Zoom mode ( last 4 are internally used only! )
        enum ZoomMode { ZoomFixed = 0, ZoomFitWidth = 1, ZoomFitPage = 2, ZoomFitText,
                        ZoomIn, ZoomOut, ZoomRefreshCurrent };
        enum MouseMode { MouseNormal, MouseZoom, MouseSelect, MouseImageSelect, MouseTextSelect, MouseTableSelect };

        // create actions that interact with this widget
        void setupBaseActions( KActionCollection * collection );
        void setupActions( KActionCollection * collection );

        // misc methods (from RMB menu/children)
        bool canFitPageWidth() const;
        void fitPageWidth( int page );
        // keep in sync with pageviewutils
        void displayMessage( const QString & message, const QString & details = QString(), PageViewMessage::Icon icon=PageViewMessage::Info, int duration=-1 );

        // inherited from DocumentObserver
        uint observerId() const { return PAGEVIEW_ID; }
        void notifySetup( const QVector< Okular::Page * > & pages, int setupFlags );
        void notifyViewportChanged( bool smoothMove );
        void notifyPageChanged( int pageNumber, int changedFlags );
        void notifyContentsCleared( int changedFlags );
        void notifyZoom(int factor);
        bool canUnloadPixmap( int pageNum ) const;

        // inherited from View
        uint viewId() const { return observerId(); }
        bool supportsCapability( ViewCapability capability ) const;
        CapabilityFlags capabilityFlags( ViewCapability capability ) const;
        QVariant capability( ViewCapability capability ) const;
        void setCapability( ViewCapability capability, const QVariant &option );

        QList< Okular::RegularAreaRect * > textSelections( const QPoint& start, const QPoint& end, int& firstpage );
        Okular::RegularAreaRect * textSelectionForItem( PageViewItem * item, const QPoint & startPoint = QPoint(), const QPoint & endPoint = QPoint() );

        void reparseConfig();

        KAction *toggleFormsAction() const;

        int contentAreaWidth() const;
        int contentAreaHeight() const;
        QPoint contentAreaPosition() const;
        QPoint contentAreaPoint( const QPoint & pos ) const;

    public slots:
        void errorMessage( const QString & message, int duration = -1 )
        {
            displayMessage( message, QString(), PageViewMessage::Error, duration );
        }

        void noticeMessage( const QString & message, int duration = -1 )
        {
            displayMessage( message, QString(), PageViewMessage::Info, duration );
        }

        void warningMessage( const QString & message, int duration = -1 )
        {
            displayMessage( message, QString(), PageViewMessage::Warning, duration );
        }

        void copyTextSelection() const;
        void selectAll();

        void setAnnotationWindow( Okular::Annotation *annotation );

        void removeAnnotationWindow( Okular::Annotation *annotation );

    signals:
        void urlDropped( const KUrl& );
        void rightClick( const Okular::Page *, const QPoint & );

    protected:
        void resizeEvent( QResizeEvent* );

        // mouse / keyboard events
        void keyPressEvent( QKeyEvent* );
        void keyReleaseEvent( QKeyEvent* );
        void inputMethodEvent( QInputMethodEvent * );
        void wheelEvent( QWheelEvent* );

        // drag and drop related events
        void dragEnterEvent( QDragEnterEvent* );
        void dragMoveEvent( QDragMoveEvent* );
        void dropEvent( QDropEvent* );

        void paintEvent( QPaintEvent *e );
        void mouseMoveEvent( QMouseEvent *e );
        void mousePressEvent( QMouseEvent *e );
        void mouseReleaseEvent( QMouseEvent *e );
        void mouseDoubleClickEvent( QMouseEvent *e );

        bool viewportEvent( QEvent *e );

        void scrollContentsBy( int dx, int dy );

    private:
        // draw background and items on the opened qpainter
        void drawDocumentOnPainter( const QRect & pageViewRect, QPainter * p );
        // update item width and height using current zoom parameters
        void updateItemSize( PageViewItem * item, int columnWidth, int rowHeight );
        // return the widget placed on a certain point or 0 if clicking on empty space
        PageViewItem * pickItemOnPoint( int x, int y );
        // extract text from a rectangular region
        const QString rectExtractText(const QRect &selectionRect, const Okular::TextPage::TextAreaInclusionBehaviour b);
        // start / modify / clear selection rectangle
        void selectionStart( const QPoint & pos, const QColor & color, bool aboveAll = false );
        void selectionEndPoint( const QPoint & pos );
        void selectionClear();
        void drawTableDividers(QPainter * screenPainter);
        // update internal zoom values and end in a slotRelayoutPages();
        void updateZoom( ZoomMode newZm );
        // update the text on the label using global zoom value or current page's one
        void updateZoomText();
        void textSelectionClear();
        // updates cursor
        void updateCursor( const QPoint &p );

        int viewColumns() const;

        void center(int cx, int cy);

        void toggleFormWidgets( bool on );

        void resizeContentArea( const QSize & newSize );
        void updatePageStep();

        void addWebShortcutsMenu( KMenu * menu, const QString & text );

        // don't want to expose classes in here
        class PageViewPrivate * d;

    private slots:
        // activated either directly or via queued connection on notifySetup
        void slotRelayoutPages();
        // activated by the resize event delay timer
        void delayedResizeEvent();
        // activated either directly or via the contentsMoving(int,int) signal
        void slotRequestVisiblePixmaps( int newValue = -1 );
        // activated by the viewport move timer
        void slotMoveViewport();
        // activated by the autoscroll timer (Shift+Up/Down keys)
        void slotAutoScoll();
        // activated by the dragScroll timer
        void slotDragScroll();
        // show the welcome message
        void slotShowWelcome();
        // activated by left click timer
        void slotShowSizeAllCursor();

        void slotHandleWebShortcutAction();
        void slotConfigureWebShortcuts();

        // connected to local actions (toolbar, menu, ..)
        void slotZoom();
        void slotZoomIn();
        void slotZoomOut();
        void slotFitToWidthToggled( bool );
        void slotFitToPageToggled( bool );
        void slotFitToTextToggled( bool );
        void slotViewMode( QAction *action );
        void slotContinuousToggled( bool );
        void slotSetMouseNormal();
        void slotSetMouseZoom();
        void slotSetMouseSelect();
        void slotSetMouseTextSelect();
        void slotSetMouseTableSelect();
        void slotToggleAnnotator( bool );
        void slotScrollUp();
        void slotScrollDown();
        void slotRotateClockwise();
        void slotRotateCounterClockwise();
        void slotRotateOriginal();
        void slotPageSizes( int );
        void slotTrimMarginsToggled( bool );
        void slotToggleForms();
        void slotFormWidgetChanged( FormWidgetIface *w );
        void slotRefreshPage();
        void slotSpeakDocument();
        void slotSpeakCurrentPage();
        void slotStopSpeaks();
        void slotAction( Okular::Action *action );
        void slotFormFieldChanged( Okular::FormField *formField );
        void externalKeyPressEvent( QKeyEvent *e );
};

#endif
