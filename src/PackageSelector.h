/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                        (C) SuSE GmbH |
\----------------------------------------------------------------------/

   File:       PackageSelector.h

   Author:     Gabriele Strattner <gs@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef PackageSelector_h
#define PackageSelector_h

#include <iosfwd>

#include "YNCursesUI.h"
#include "PkgNames.h"

#include <map>          
#include <string>
#include <utility>      // for STL pair

#include <y2util/YRpmGroupsTree.h>

#include <zypp/ResObject.h>
#include <zypp/ui/Selectable.h>
#include <zypp/Patch.h>

#include "NCPopupTree.h"
#include "NCPkgTable.h"

class NCPopupSelection;
class LangCode;
class NCPopupDeps;
class NCPopupDiskspace;
class NCPopupSearch;
class NCPopupFile;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : PackageSelector
//
//	DESCRIPTION : holds the data and handles events
//
class PackageSelector
{

  friend std::ostream & operator<<( std::ostream & STREAM, const PackageSelector & OBJ );

  PackageSelector & operator=( const PackageSelector & );
  PackageSelector            ( const PackageSelector & );

  private:

    // typedef for the pointer to handler member function
    typedef bool (PackageSelector::* tHandlerFctPtr) ( const NCursesEvent& event );

    // typedef for the internal map: key=nameId, value=handler-fct-ptr
    typedef std::map<std::string, tHandlerFctPtr> tHandlerMap;

    tHandlerMap eventHandlerMap;    	// event handler map
    
    YNCursesUI * y2ui;			// the UI

    YCPValue visibleInfo;		// visible package info (description, file list, ...)

    NCPopupTree * filterPopup;		// the rpm group tags popup

    NCPopupDeps * depsPopup;	// the package dependeny popup

#ifdef FIXME
    NCPopupSelection * selectionPopup; 	// the selections popup

    NCPopupDiskspace * diskspacePopup;	// the popup showing the disk usage
#endif

    NCPopupSearch * searchPopup; 	// the package search popup

#ifdef FIXME
    NCPopupFile * filePopup; 		// the save/load selection popup
#endif
    
    bool youMode;			// YOU
    bool updateMode;			// Update

    bool autoCheck;			// flag for automatic dependency check on/off
    
#ifdef FIXME
    // internal helper functions (format list of string) 
    string createRelLine( list<PkgRelation> info );
#endif
    // internal use (copies tree items got from YPkgRpmGroupTagsFilterView)
    void cloneTree( YStringTreeItem * parentOrig, YTreeItem * parentClone );

    // the package (patch) list 
    NCPkgTable * getPackageList();
    
  protected:
 

  public:

   /**
     * The package selector class handles the events and holds the
     * data needed for the package selection.
     * @param ui The NCurses UI
     * @param opt The widget options
     * @param floppyDevice The floppy device
     */
    PackageSelector( YNCursesUI * ui, const YWidgetOpt & opt, string floppyDevice );

    /**
     * Destructor
     */ 
    virtual ~PackageSelector();
    
   /**
    * Fills the package table
    * @param label The selected RPM group (the label)
    * @param group The rpm group
    * @return bool
    */
    bool fillPackageList( const YCPString & label, YStringTreeItem * group );

   /**
    * Fills the package table
    * Temporary while porting to zypp
    * @param label (the label)
    * @return bool
    */
    bool fillPackageListAll (const YCPString & label);

#ifdef FIXME_PATCHES
  /**
    * Fills the package table with YOU patches matching the filter
    * @param filter
    * @return bool
    */
    bool fillPatchList( string filter,
			zypp::Patch::Kind kind = zypp::Patch::kind_all);
#endif

   /**
    * Fills the package table with packages with update problems
    * @return bool
    */
    bool fillUpdateList( );

  /**
    * Fills the package table with a summary packages 
    * @return bool
    */
    bool fillSummaryList( NCPkgTable::NCPkgTableListType type );
    
   /**
    * Fills the list of available packages
    * @param table  The table widget
    * @param pkgPtr Show all available versions of this package 
    * @return bool
    */
    bool fillAvailableList( NCPkgTable *table, ZyppObj pkgPtr );    

   /**
    * Fills the list of packages belonging to the youPatch
    * @param pkgTable  The table widget
    * @param youPatch Show all packages belonging to the patch 
    * @return bool
    */ 
    bool fillPatchPackages ( NCPkgTable * pkgTable, ZyppObj youPatch );
    
   /**
    * Fills the package table with packages matching the search expression
    * @param expr The search expression
    * @param ignoreCase Ignore case (true or false)
    * @param checkName Search in package name (true or false)
    * @param checkSummary Check the summary (true or false)
    * @param checkProvides Check in Provides (true or false)
    * @param checkRequires Check in Requires (true or false)
    * @return bool
    */ 
    bool fillSearchList( const YCPString & expr,
			 bool ignoreCase,
			 bool checkName,
			 bool checkSummary,
			 bool checkDescr,
			 bool checkProvides,
			 bool checkRequires );

   /**
    * Gets default RPM group (the first group)
    * @return YStringTreeItem *
    */
    YStringTreeItem * getDefaultRpmGroup() { return filterPopup->getDefaultGroup(); }
    
    /**
     * Handle the given event. For the given event (the widget-id
     * is contained in the event) the corresponding handler is executed.
     * @param event The NCurses event
     * @return bool
     */
    bool handleEvent( const NCursesEvent& event );

   /**
    * Handler function for "Search button pressed"
    * @param event The Ncurses event
    * @return bool
    */
    bool SearchHandler ( const NCursesEvent& event );

    /**
    * Handler function for menu selection "Etc./Check dependencies"
    * @param event The Ncurses event
    * @return bool
    */
    bool DependencyHandler( const NCursesEvent&  event );

   /**
    * Handler function for menu selection "Etc./Selections"
    * @param event The Ncurses event
    * @return bool
    */
    bool SelectionHandler( const NCursesEvent&  event );
    
    /**
     * Handler function for the "Information" menu
     * @param event The Ncurses event
     * @return bool
     */
    bool InformationHandler ( const NCursesEvent& event );

    /**
    * Handler function for "OK button pressed"
    * @param event The Ncurses event
    * @return bool 
    */
    bool OkButtonHandler ( const NCursesEvent& event );

   /**
    * Handler function for "Cancel button pressed"
    * @param event The Ncurses event
    * @return bool
    */
    bool CancelHandler ( const NCursesEvent& event );

    /**
    * Handler function for "Diskspace button pressed"
    * @param event The Ncurses event
    * @return bool
    */
    bool DiskinfoHandler ( const NCursesEvent& event );
    
    /**
     * Handler function for the package list which handles events not directly
     * related to the list. Most events are handled in NCPkgTable itself.
     * @param event The Ncurses event
     * @return bool
     */ 
    bool PackageListHandler( const NCursesEvent&  event );

    /**
     * Handler function for the "Filter" menu
     * @param event The Ncurses event
     * @return bool     
     */
    bool FilterHandler( const NCursesEvent&  event );

    /**
     * Handler function for the "Action" menu (changes the package status)
     * @param event The Ncurses event
     * @return bool     
     */
    bool StatusHandler( const NCursesEvent&  event );

   /**
    * Handler function for the "Help" menu
    * @param event The Ncurses event
    * @return bool
    */
    bool HelpHandler( const NCursesEvent&  event );

  /**
    * Handler function for the "Help" button in YOU
    * @param event The Ncurses event
    * @return bool
    */
    bool YouHelpHandler( const NCursesEvent&  event );

   /**
     * Handles hyperlinks in package description
     * @param link The link
     * @return bool
     */
    bool LinkHandler ( string link );
    
   /**
    * Gets the required package info from package manager and shows it
    * (	called from NCPkgTable )
    * @param pkgPtr the data pointer
    * @return bool
    */
    bool showPackageInformation ( ZyppObj pkgPtr, ZyppSel slbPtr );

  /**
    * Checks and shows the dependencies
    * @param doit true: do the check, false: only check if auto check is on
    */
    bool showPackageDependencies ( bool doit );

    /**
    * Checks and shows the selectiondependencies
    */
    void showSelectionDependencies ( );
    
#ifdef FIXME
   /**
    * Gets the required patch info from you patch manager and shows it
    * @param pkgPtr the data pointer
    * @return bool
    */
    bool showPatchInformation ( ZyppObj pkgPtr );
#endif
    
   /**
    * Sets the member variable to the currently visible information
    * @param info
    */
    void setVisibleInfo( const YCPValue & info );

   /**
    * Fills the package table
    * @param label The selected selection (the label)
    * @param selPtr The selection
    * @return bool
    */
    bool showSelPackages( const YCPString & label, ZyppSelection selPtr );

   /**
    * Updates the status in list of packages
    */
    void updatePackageList();
    
    /**
     * Check if 'pkg' matches 'selectedRpmGroup'.
     * Returns true if there is a match, false otherwise or if 'pkg' is 0.
     * @return bool 
     **/
    bool checkPackage( ZyppPkg pkg, YStringTreeItem * rpmGroup );

    /**
     * Check if 'patch' matches the selected filter.
     * Returns true if there is a match, false otherwise or if 'patch' is 0.
     * @return bool
     **/ 
    bool checkPatch(  ZyppPatch patch, string filter, zypp::Patch::Kind kind );

   /**
    * Returns whether automatic dependency is on or off
    * @return bool   
    */ 
    bool autoChecking() { return autoCheck; }

    /**
     * Creates a text from a list of strings
     * @param oneline	true: create one line, items seperated by comma; false: every string is a line
     * @return string	The text
     */
    string createText( list<string> info, bool oneline );

   /**
     * Creates a text from a list of strings which may contain HTML tags
     * @param t used to be list, now a single string. but what does it contain?
     * @return string	The text
     */
    string createDescrText( zypp::Text t );

    /**
     * Used for package search
     * @param s1 Search in s1
     * @param s2 Searching for s2
     * @return bool
     */
    bool match( string s1, string s2, bool ignoreCase );

    /**
     * Calls the package mananager (updateDu()) and shows the required disk space
     */ 
    void showDiskSpace();

#ifdef FIXME
    /**
     * Shows the total download size
     */ 
    void showDownloadSize();
#endif

};

///////////////////////////////////////////////////////////////////

#endif // PackageSelector_h
