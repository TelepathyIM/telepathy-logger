#include "config.h"

#include "lib/util.h"

#include "telepathy-logger/call-event.h"
#include "telepathy-logger/debug-internal.h"
#include "telepathy-logger/log-iter-internal.h"
#include "telepathy-logger/log-iter-xml-internal.h"
#include "telepathy-logger/log-store-xml-internal.h"
#include "telepathy-logger/text-event.h"

#include <telepathy-glib/telepathy-glib.h>
#include <glib.h>

#define DEBUG_FLAG TPL_DEBUG_TESTSUITE


typedef struct
{
  TplLogStore *store;
  TpAccount *account;
  TpDBusDaemon *bus;
  TpSimpleClientFactory *factory;
} XmlTestCaseFixture;


static void
setup (XmlTestCaseFixture* fixture,
    gconstpointer user_data)
{
  GError *error = NULL;

  fixture->store = g_object_new (TPL_TYPE_LOG_STORE_XML,
      "testmode", TRUE,
      NULL);

  fixture->bus = tp_tests_dbus_daemon_dup_or_die ();
  g_assert (fixture->bus != NULL);

  fixture->factory = tp_simple_client_factory_new (fixture->bus);
  g_assert (fixture->factory != NULL);

  /* We can get away without preparing the account */
  fixture->account = tp_simple_client_factory_ensure_account (fixture->factory,
      TP_ACCOUNT_OBJECT_PATH_BASE "gabble/jabber/user_40collabora_2eco_2euk",
      NULL,
      &error);
  g_assert_no_error (error);
  g_assert (fixture->account != NULL);

  tp_debug_divert_messages (g_getenv ("TPL_LOGFILE"));

#ifdef ENABLE_DEBUG
  _tpl_debug_set_flags_from_env ();
#endif /* ENABLE_DEBUG */
}


static void
teardown (XmlTestCaseFixture *fixture,
    gconstpointer user_data)
{
  g_clear_object (&fixture->account);
  g_clear_object (&fixture->factory);
  g_clear_object (&fixture->bus);
  g_clear_object (&fixture->store);
}


static void
test_get_events (XmlTestCaseFixture *fixture,
    gconstpointer user_data)
{
  TplEntity *user2, *user4, *user6;
  TplLogIter *iter;
  GList *events;
  GError *error = NULL;
  GTimeSpan duration;
  const gchar *message;
  gint64 timestamp;

  user2 = tpl_entity_new ("user2@collabora.co.uk", TPL_ENTITY_CONTACT,
      "User2", "");

  user4 = tpl_entity_new ("user4@collabora.co.uk", TPL_ENTITY_CONTACT,
      "User4", "");

  user6 = tpl_entity_new ("user6@collabora.co.uk", TPL_ENTITY_CONTACT,
      "User6", "");

  /* Text events spanning multiple days */
  iter = tpl_log_iter_xml_new (fixture->store, fixture->account, user2,
      TPL_EVENT_MASK_ANY);

  events = tpl_log_iter_get_events (iter, 5, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 5);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266425566);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "4");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 3, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 3);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266425572);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "3");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 2, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 2);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266425566);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "5");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 7, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 7);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266414451);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "1");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 1, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 1);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266335850);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "bar");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 2, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 2);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266335556);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "1");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 10, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 10);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1263405178);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "5");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 4, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 4);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1263404877);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "1");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 3, &error);
  g_assert_no_error (error);
  g_assert (events == NULL);

  g_object_unref (iter);

  /* A mix of call and text events */
  iter = tpl_log_iter_xml_new (fixture->store, fixture->account, user4,
      TPL_EVENT_MASK_ANY);

  events = tpl_log_iter_get_events (iter, 4, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 4);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1263404881);
  duration = tpl_call_event_get_duration (TPL_CALL_EVENT (events->data));
  g_assert_cmpint (duration, ==, 1);
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 1, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 1);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1263404881);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "8");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 1, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 1);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1263404877);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "7");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 1, &error);
  g_assert_no_error (error);
  g_assert (events == NULL);

  g_object_unref (iter);

  /* Files with invalid XML */
  iter = tpl_log_iter_xml_new (fixture->store, fixture->account, user6,
      TPL_EVENT_MASK_ANY);

  events = tpl_log_iter_get_events (iter, 2, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 2);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1388680071);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "34");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 1, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 1);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1388680070);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 2, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 2);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1388568367);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "1");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 2, &error);
  g_assert_no_error (error);
  g_assert (events == NULL);

  g_object_unref (iter);

  g_object_unref (user2);
  g_object_unref (user4);
  g_object_unref (user6);
}


static void
test_rewind (XmlTestCaseFixture *fixture,
    gconstpointer user_data)
{
  TplEntity *user2, *user4;
  TplLogIter *iter;
  GList *events;
  GError *error = NULL;
  GTimeSpan duration;
  const gchar *message;
  gint64 timestamp;

  user2 = tpl_entity_new ("user2@collabora.co.uk", TPL_ENTITY_CONTACT,
      "User2", "");

  user4 = tpl_entity_new ("user4@collabora.co.uk", TPL_ENTITY_CONTACT,
      "User4", "");

  /* Text events spanning multiple days */
  iter = tpl_log_iter_xml_new (fixture->store, fixture->account, user2,
      TPL_EVENT_MASK_ANY);

  tpl_log_iter_rewind (iter, 8, &error);
  g_assert_no_error (error);

  events = tpl_log_iter_get_events (iter, 0, &error);
  g_assert_no_error (error);
  g_assert (events == NULL);

  tpl_log_iter_rewind (iter, 8, &error);
  g_assert_no_error (error);

  events = tpl_log_iter_get_events (iter, 5, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 5);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266425566);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "4");
  g_list_free_full (events, g_object_unref);

  tpl_log_iter_rewind (iter, 8, &error);
  g_assert_no_error (error);

  events = tpl_log_iter_get_events (iter, 10, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 10);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266425566);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "5");
  g_list_free_full (events, g_object_unref);

  tpl_log_iter_rewind (iter, 3, &error);
  g_assert_no_error (error);

  events = tpl_log_iter_get_events (iter, 5, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 5);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266425566);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "3");
  g_list_free_full (events, g_object_unref);

  tpl_log_iter_rewind (iter, 1, &error);
  g_assert_no_error (error);

  tpl_log_iter_rewind (iter, 9, &error);
  g_assert_no_error (error);

  events = tpl_log_iter_get_events (iter, 10, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 10);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266425566);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "3");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 10, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 10);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266329628);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "123");
  g_list_free_full (events, g_object_unref);

  tpl_log_iter_rewind (iter, 13, &error);
  g_assert_no_error (error);

  events = tpl_log_iter_get_events (iter, 10, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 10);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1266335803);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "a");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 10, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 10);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1263405203);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "6");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 5, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 5);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1263404877);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "1");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 3, &error);
  g_assert_no_error (error);
  g_assert (events == NULL);

  g_object_unref (iter);

  /* A mix of call and text events */
  iter = tpl_log_iter_xml_new (fixture->store, fixture->account, user4,
      TPL_EVENT_MASK_ANY);

  tpl_log_iter_rewind (iter, 8, &error);
  g_assert_no_error (error);

  events = tpl_log_iter_get_events (iter, 0, &error);
  g_assert_no_error (error);
  g_assert (events == NULL);

  tpl_log_iter_rewind (iter, 8, &error);
  g_assert_no_error (error);

  events = tpl_log_iter_get_events (iter, 4, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 4);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1263404881);
  duration = tpl_call_event_get_duration (TPL_CALL_EVENT (events->data));
  g_assert_cmpint (duration, ==, 1);
  g_list_free_full (events, g_object_unref);

  tpl_log_iter_rewind (iter, 8, &error);
  g_assert_no_error (error);

  events = tpl_log_iter_get_events (iter, 4, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 4);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1263404881);
  duration = tpl_call_event_get_duration (TPL_CALL_EVENT (events->data));
  g_assert_cmpint (duration, ==, 1);
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 2, &error);
  g_assert_no_error (error);
  g_assert (events != NULL);
  g_assert_cmpint (g_list_length (events), ==, 2);
  timestamp = tpl_event_get_timestamp (TPL_EVENT (events->data));
  g_assert_cmpint (timestamp, ==, 1263404877);
  message = tpl_text_event_get_message (TPL_TEXT_EVENT (events->data));
  g_assert_cmpstr (message, ==, "7");
  g_list_free_full (events, g_object_unref);

  events = tpl_log_iter_get_events (iter, 1, &error);
  g_assert_no_error (error);
  g_assert (events == NULL);

  g_object_unref (iter);

  g_object_unref (user2);
  g_object_unref (user4);
}


gint main (gint argc, gchar **argv)
{
  g_type_init ();

  g_test_init (&argc, &argv, NULL);
  g_test_bug_base ("http://bugs.freedesktop.org/show_bug.cgi?id=");

  g_test_add ("/log-iter-xml/get-events",
      XmlTestCaseFixture, NULL,
      setup, test_get_events, teardown);

  g_test_add ("/log-iter-xml/rewind",
      XmlTestCaseFixture, NULL,
      setup, test_rewind, teardown);

  return g_test_run ();
}
