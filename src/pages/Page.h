// SWS standard page
#pragma once

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

#define PAGE_CONTROLLER 0
#define PAGE_MOUNT 1
#define PAGE_ROTATOR 2
#define PAGE_FOCUSER 3
#define PAGE_AUXILIARY 4
#define PAGE_ENCODERS 5
#define PAGE_NETWORK 6

#include "../extensions/ui/Presentation.h"
#include "../extensions/ui/Icons.h"
void pageHeader(int selected);

const char html_onstep_page_begin[] PROGMEM = "<div class='b'>\n";

const char html_links_idx_begin[] PROGMEM = "<a href='/index.htm' ";
const char html_links_idx_end[] PROGMEM = ">" SWS_NAV_ICON(index) "<span>" L_CONTROLLER "</span></a>";

const char html_links_mnt_begin[] PROGMEM = "<a href='/mount.htm' ";
const char html_links_mnt_end[] PROGMEM = ">" SWS_NAV_ICON(mount) "<span>" L_PAGE_MOUNT "</span></a>";

const char html_links_rot_begin[] PROGMEM = "<a href='/rotator.htm' ";
const char html_links_rot_end[] PROGMEM = ">" SWS_NAV_ICON(rotator) "<span>" L_ROTATOR "</span></a>";

const char html_links_foc_begin[] PROGMEM = "<a href='/focuser.htm' ";
const char html_links_foc_end[] PROGMEM = ">" SWS_NAV_ICON(focuser) "<span>" L_FOCUSER "</span></a>";

const char html_links_aux_begin[] PROGMEM = "<a href='/auxiliary.htm' ";
const char html_links_aux_end[] PROGMEM = ">" SWS_NAV_ICON(auxiliary) "<span>" L_PAGE_AUXILIARY "</span></a>";

const char html_links_enc_begin[] PROGMEM = "<a href='/enc.htm' ";
const char html_links_enc_end[] PROGMEM = ">" SWS_NAV_ICON(enc) "<span>" L_PAGE_ENCODERS "</span></a>";

const char html_links_net_begin[] PROGMEM = "<a href='/net.htm' ";
const char html_links_net_end[] PROGMEM = ">" SWS_NAV_ICON(net) "<span>" L_PAGE_NETWORK "</span></a>";

const char html_links_selected[] PROGMEM = " aria-current='page' ";
