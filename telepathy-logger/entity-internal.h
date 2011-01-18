/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 *Copyright (C) 2009-2010 Collabora Ltd.
 *
 *This library is free software; you can redistribute it and/or
 *modify it under the terms of the GNU Lesser General Public
 *License as published by the Free Software Foundation; either
 *version 2.1 of the License, or (at your option) any later version.
 *
 *This library is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *Lesser General Public License for more details.
 *
 *You should have received a copy of the GNU Lesser General Public
 *License along with this library; if not, write to the Free Software
 *Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *Authors: Cosimo Alfarano <cosimo.alfarano@collabora.co.uk>
 */

#ifndef __TPL_ENTITY_INTERNAL_H__
#define __TPL_ENTITY_INTERNAL_H__

#include <telepathy-logger/entity.h>

#include <glib-object.h>
#include <telepathy-glib/contact.h>

G_BEGIN_DECLS
#define TPL_ENTITY_CLASS(klass)  (G_TYPE_CHECK_CLASS_CAST ((klass), TPL_TYPE_ENTITY, TplEntityClass))
#define TPL_IS_ENTITY_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TPL_TYPE_ENTITY))
#define TPL_ENTITY_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TPL_TYPE_ENTITY, TplEntityClass))

typedef struct
{
  GObjectClass parent_class;
} TplEntityClass;

TplEntity *_tpl_entity_new_from_tp_contact (TpContact *contact, TplEntityType type);
TplEntity *_tpl_entity_new_from_room_id (const gchar *room_id);

G_END_DECLS
#endif // __TPL_ENTITY_INTERNAL_H__
