/*****************************************************************************
 * vlc_streaming.h: Methods and descriptions for Streaming profiles
 *****************************************************************************
 * Copyright (C) 2002-2005 the VideoLAN team
 * $Id: stream_output.h 14151 2006-02-04 00:08:50Z zorglub $
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef _VLC_STREAMING_H_
#define _VLC_STREAMING_H_

#include <vlc/vlc.h>

/***********************************************************************
 * Predefined lists of streaming data
 ***********************************************************************/

#ifdef WIN32
#define VCODECS_NUMBER 13
#else
#define VCODECS_NUMBER 12
#endif

#define ACODECS_NUMBER 9

/// Keep this up to date !
#define MUXERS_NUMBER 10
enum
{
    MUX_PS, MUX_TS, MUX_MPEG, MUX_OGG, MUX_RAW,
    MUX_ASF, MUX_AVI, MUX_MP4, MUX_MOV, MUX_WAV,
};

enum { ACCESS_HTTP, ACCESS_UDP, ACCESS_MMS, ACCESS_RTP, ACCESS_FILE };

struct codec_desc_t {
    char *psz_display;
    char *psz_codec;
    char *psz_descr;
    int muxers[MUXERS_NUMBER];
};
//extern const struct codec vcodecs_array[];
//extern const struct codec acodecs_array[];

struct method_desc_t {
    char *psz_access;
    char *psz_method;
    char *psz_descr;
    char *psz_address;
    int   muxers[MUXERS_NUMBER];
};
//xtern const struct method methods_array[];

struct mux_desc_t {
    int   id;
    char *psz_mux;
    char *psz_encap;
    char *psz_descr;
};
//extern const struct mux_desc_t muxers_array[];


/* Standard bitrates arrays */
//static const char * vbitrates_array[] =
//{ "3072", "2048", "1024", "768", "512", "384", "256", "192", "128", "96",
//  "64" };

//static const char *abitrates_array[] =
//{ "512", "256", "192", "128", "96", "64", "32", "16" } ;
//


/***********************************************************************
 * Streaming profiles
 ***********************************************************************/

/****************** Parameters handling *********************/
struct sout_param_t
{
    int i_type;
    int i_element;
    char *psz_id;
    char *psz_string;
    vlc_value_t value;
};

struct sout_pcat_t
{
    char *psz_name;
    int i_params;
    sout_param_t **pp_params;
};

void streaming_ParameterApply( sout_param_t *p_param, char **ppsz_dest,
                           int *pi_dest, float *pf_dest, vlc_bool_t *pb_dest );


/******** Module types definitions and parametrable elements ***************/

/* Transcode */
enum { I_VB, I_AB, I_CHANNELS, F_SCALE, B_SOVERLAY, PSZ_VC, PSZ_AC, PSZ_SC,
       PSZ_VE, PSZ_AE };
struct sout_transcode_t
{
    int i_vb, i_ab, i_channels;
    float f_scale;
    vlc_bool_t b_soverlay;
    char *psz_vcodec, *psz_acodec, *psz_scodec, *psz_venc, *psz_aenc;
    char *psz_additional;

    int i_params; sout_param_t **pp_params;
};
void streaming_TranscodeParametersApply( sout_transcode_t *p_module );

/* Standard */
enum { PSZ_MUX, PSZ_ACCESS, PSZ_URL, PSZ_NAME, PSZ_GROUP };
struct sout_std_t
{
    char *psz_mux, *psz_access;
    char *psz_url, *psz_name, *psz_group;
    int i_params; sout_param_t **pp_params;
};
void streaming_StdParametersApply( sout_std_t *p_module );

/* Display */
struct sout_display_t
{
};

/* Duplicate */
struct sout_duplicate_t
{
    int i_children, i_conditions;
    sout_chain_t **pp_children;
    char **ppsz_conditions;
};

/******* Generic profile structures and manipulation functions ********/
typedef union
{
    sout_transcode_t *p_transcode;
    sout_std_t       *p_std;
    sout_duplicate_t *p_duplicate;
    sout_display_t   *p_display;
} sout_module_type_t;

struct sout_module_t
{
    int i_type;
    sout_module_type_t typed;
    sout_module_t *p_parent;
};

enum { SOUT_MOD_TRANSCODE, SOUT_MOD_STD, SOUT_MOD_RTP, SOUT_MOD_DUPLICATE,
       SOUT_MOD_DISPLAY };

struct sout_chain_t
{
    int i_modules;
    sout_module_t **pp_modules;

    int i_options;
    char **ppsz_options;

    int i_pcats;
    sout_pcat_t **pp_pcats;
};

static inline sout_chain_t *streaming_ChainNew(void)
{
    DECMALLOC_NULL( p_chain, sout_chain_t );
    memset( p_chain, 0, sizeof( sout_chain_t ) );
    p_chain->i_options = 0;
    return p_chain;
}

struct streaming_profile_t
{
    char *psz_title;
    char *psz_description;
    sout_chain_t *p_chain;
};

/**************** GUI interaction *****************/
struct sout_gui_descr_t
{
    /* Access */
    vlc_bool_t b_local, b_file, b_http, b_mms, b_rtp, b_udp, b_dump;
    char *psz_file, *psz_http, *psz_mms, *psz_rtp, *psz_udp;
    int i_http, i_mms, i_rtp, i_udp;

    /* Mux */
    char *psz_mux;

    /* Transcode */
    vlc_bool_t b_soverlay;
    char *psz_vcodec, *psz_acodec, *psz_scodec;
    int i_vb, i_ab, i_channels;
    float f_scale;

    /* Misc */
    vlc_bool_t b_sap, b_all_es;
    char *psz_group, *psz_name;
    int i_ttl;
};

/***************** Profile parsing ***********************/

struct profile_parser_t
{
    char *psz_profile;
    streaming_profile_t *p_profile;
};

#endif
