
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/



#define ROCK_TYPES 35
#define OFFSHORE_TYPES 22
#define LIGHT_TYPES 47


QString rockType[ROCK_TYPES] = {
  optionPage::tr ("Rock (islet) which does not cover"),
  optionPage::tr ("Rock which covers and uncovers"),
  optionPage::tr ("Rock awash at the level of chart datum"),
  optionPage::tr ("Dangerous underwater rock of uncertain depth"),
  optionPage::tr ("Dangerous underwater rock of known depth"),
  optionPage::tr ("Non-dangerous rock, depth known"),
  optionPage::tr ("Coral reef which covers and uncovers"),
  optionPage::tr ("Coral reef awash at level of chart datum"),
  optionPage::tr ("Dangerous underwater coral reef of uncertain depth"),
  optionPage::tr ("Dangerous underwater coral reef of known depth"),
  optionPage::tr ("Non-dangerous coral reef, depth known"),
  optionPage::tr ("Breakers"),
  optionPage::tr ("Wreck, hull always dry"),
  optionPage::tr ("Wreck, covers and uncovers"),
  optionPage::tr ("Submerged wreck, depth known"),
  optionPage::tr ("Submerged wreck, depth unknown"),
  optionPage::tr ("Wreck showing any portion of hull or superstructure at level of chart datum"),
  optionPage::tr ("Wreck showing mast or masts above chart datum only"),
  optionPage::tr ("Wreck, least depth known by sounding only"),
  optionPage::tr ("Wreck, least depth known, swept by wire drag or diver"),
  optionPage::tr ("Dangerous wreck, depth unknown"),
  optionPage::tr ("Sunken wreck, not dangerous to surface navigation"),
  optionPage::tr ("Wreck, least depth unknown, but considered to have a safe clearance to the depth shown"),
  optionPage::tr ("Foul ground, non-dangerous to navigation but to be avoided by vessels anchoring, trawling, etc"),
  optionPage::tr ("Foul area, foul with rocks or wreckage, dangerous to navigation"),
  optionPage::tr ("Obstruction, depth unknown"),
  optionPage::tr ("Obstruction, least depth known"),
  optionPage::tr ("Obstruction, least depth known, swept by wire drag or diver"),
  optionPage::tr ("Stumps of posts or piles, all or part of the time submerged"),
  optionPage::tr ("Submerged pile, stake, snag, well, deadhead, or stump"),
  optionPage::tr ("Fishing stakes"),
  optionPage::tr ("Fish trap, fish weirs, tunny nets"),
  optionPage::tr ("Fish haven (artificial fishing reef)"),
  optionPage::tr ("Fish haven with minimum depth"),
  optionPage::tr ("Shellfish culivation (stakes visible)")
};


QString offshoreType[OFFSHORE_TYPES] = {
  optionPage::tr ("Production platform, platform, oil derrick"),
  optionPage::tr ("Flare stack (at sea)"),
  optionPage::tr ("Mooring tower, Articulated Loading Platform (ALP), Single Anchor Leg Mooring (SALM)"),
  optionPage::tr ("Observation/research platform"),
  optionPage::tr ("Disused platform"),
  optionPage::tr ("Artificial island"),
  optionPage::tr ("Oil or gas installation buoy, Catenary Anchor Leg Mooring (CALM), Single Buoy Mooring (SBM)"),
  optionPage::tr ("Moored storage tanker"),
  optionPage::tr ("Submerged production well"),
  optionPage::tr ("Suspended well, depth over wellhead unknown"),
  optionPage::tr ("Suspended well, with depth over wellhead"),
  optionPage::tr ("Wellhead with height above the bottom"),
  optionPage::tr ("Site of cleared platform"),
  optionPage::tr ("Above water wellhead"),
  optionPage::tr ("Submarine cable"),
  optionPage::tr ("Submarine power cable"),
  optionPage::tr ("Disused submarine cable"),
  optionPage::tr ("Oil, gas pipeline"),
  optionPage::tr ("Waterpipe, sewer, outfall pipe, intake pipe"),
  optionPage::tr ("Buried pipeline pipe"),
  optionPage::tr ("Potable water intake, diffuser, or crib"),
  optionPage::tr ("Disused pipeline/pipe")
};


QString lightType[LIGHT_TYPES] = {
  optionPage::tr ("Major light, minor light, light, lighthouse"),
  optionPage::tr ("Lighted offshore platform"),
  optionPage::tr ("Lighted beacon tower"),
  optionPage::tr ("Lighted beacon"),
  optionPage::tr ("Articulated light, buoyant beacon, resilient beacon"),
  optionPage::tr ("Light vessel; Lightship, normally manned light vessel"),
  optionPage::tr ("Unmanned light vessel, light float"),
  optionPage::tr ("LANBY, superbuoy as navigational aid"),
  optionPage::tr ("Beacon"),
  optionPage::tr ("Buoy"),
  optionPage::tr ("Conical buoy, nun buoy"),
  optionPage::tr ("Can or cylindrical buoy"),
  optionPage::tr ("Spherical buoy"),
  optionPage::tr ("Pillar buoy"),
  optionPage::tr ("Spar buoy, spindle buoy"),
  optionPage::tr ("Barrel buoy"),
  optionPage::tr ("Super buoy"),
  optionPage::tr ("Mooring buoy"),
  optionPage::tr ("Lighted mooring buoy"),
  optionPage::tr ("Trot, mooring buoys with ground tackle"),
  optionPage::tr ("Firing danger area (Danger Zone) buoy"),
  optionPage::tr ("Target buoy"),
  optionPage::tr ("Marker ship"),
  optionPage::tr ("Barge"),
  optionPage::tr ("Degaussing range buoy"),
  optionPage::tr ("Cable buoy"),
  optionPage::tr ("Spoil ground buoy"),
  optionPage::tr ("Buoy marking outfall"),
  optionPage::tr ("ODAS buoy (Ocean Data Acquisition System), data collecting buoy of super buoy size"),
  optionPage::tr ("Wave recorder, current meter"),
  optionPage::tr ("Seaplane anchorage buoy"),
  optionPage::tr ("Buoy marking traffic separation scheme"),
  optionPage::tr ("Buoy marking recreation zone"),
  optionPage::tr ("Buoy privately maintained"),
  optionPage::tr ("Seasonal buoy"),
  optionPage::tr ("Beacon on submerged rock"),
  optionPage::tr ("Stake, pole"),
  optionPage::tr ("Perch, stake"),
  optionPage::tr ("Withy"),
  optionPage::tr ("Cairn"),
  optionPage::tr ("Beacon tower"),
  optionPage::tr ("Lattice tower"),
  optionPage::tr ("Leading beacon"),
  optionPage::tr ("Cable landing beacon"),
  optionPage::tr ("Refuge beacon"),
  optionPage::tr ("Firing danger area beacon"),
  optionPage::tr ("Notice board")
};
