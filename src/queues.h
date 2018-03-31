/* copyright 2013 Sascha Kruse and contributors (see LICENSE for licensing information) */

/**
 * @file queues.c
 */

#ifndef DUNST_QUEUE_H
#define DUNST_QUEUE_H

#include "dbus.h"
#include "notification.h"

/**
 * Initialise necessary queues
 *
 * @pre Do not call consecutively to avoid memory leaks
 *      or assure to have queues_teardown() executed before
 */
void queues_init(void);

/**
 * Set maximum notification count to display
 * and store in displayed queue
 *
 * @param limit The maximum amount
 */
void queues_displayed_limit(unsigned int limit);

/**
 * Receive the current list of displayed notifications
 *
 * @return read only list of notifications
 */
const GList *queues_get_displayed(void);

/**
 * Returns the current amount of notifications,
 * which are waiting to get displayed
 */
unsigned int queues_length_waiting(void);

/**
 * Returns the current amount of notifications,
 * which are shown in the UI
 */
unsigned int queues_length_displayed(void);

/**
 * Returns the current amount of notifications,
 * which are already in history
 */
unsigned int queues_length_history(void);

/**
 * Insert a fully initialized notification into queues
 *
 * Respects stack_duplicates, and notification replacement
 *
 * @param n the notification to insert
 *
 * - If n->id != 0, n replaces notification n with id n->id
 * - If n->id == 0, n gets a new id assigned
 *
 * @return `0`, the notification was dismissed and freed
 * @return The new value of `n->id`
 */
int queues_notification_insert(notification *n);

/**
 * Replace the notification which matches the id field of
 * the new notification. The given notification is inserted
 * right in the same position as the old notification.
 *
 * @param new replacement for the old notification
 *
 * @return true, if a matching notification has been found and is replaced
 * @return false, else
 */
bool queues_notification_replace_id(notification *new);

/**
 * Close the notification that has n->id == id
 *
 * Sends a signal and pushes the selected notification automatically to history.
 *
 * @param id The id of the notification to close
 * @param reason The #reason to close
 *
 * @post Call wake_up() to synchronize the queues with the UI
 *       (which closes the notification on screen)
 */
void queues_notification_close_id(int id, enum reason reason);

/**
 * Close the given notification. \see queues_notification_close_id().
 *
 * @param n (transfer full) The notification to close
 * @param reason The #reason to close
 * */
void queues_notification_close(notification *n, enum reason reason);

/**
 * Pushes the latest notification of history to the displayed queue
 * and removes it from history
 */
void queues_history_pop(void);

/**
 * Push a single notification to history
 * The given notification has to be removed its queue
 *
 * @param n (transfer full) The notification to push to history
 */
void queues_history_push(notification *n);

/**
 * Push all waiting and displayed notifications to history
 */
void queues_history_push_all(void);

/**
 * Check timeout of each notification and close it, if necessary
 *
 * @param idle the program's idle status. Important to calculate the
 *             timeout for transient notifications
 * @param fullscreen the desktop's fullscreen status. Important to
 *             calculate the timeout for transient notifications
 */
void queues_check_timeouts(bool idle, bool fullscreen);

/**
 * Move inserted notifications from waiting queue to displayed queue
 * and show them. In displayed queue, the amount of elements is limited
 * to the amount set via queues_displayed_limit()
 *
 * @post Call wake_up() to synchronize the queues with the UI
 *       (which closes old and shows new notifications on screen)
 *
 * @param fullscreen the desktop's fullscreen status. Important to
 *                   move notifications to the right queue
 */
void queues_update(bool fullscreen);

/**
 * Calculate the distance to the next event, when an element in the
 * queues changes
 *
 * @param time the current time
 *
 * @return the distance to the next event in the queue, which forces
 *         an update visible to the user. This may be:
 *             - notification hits timeout
 *             - notification's age second changes
 *             - notification's age threshold is hit
 */
gint64 queues_get_next_datachange(gint64 time);

/**
 * Pause queue-management of dunst
 *
 * @post Calling update_lists() is neccessary
 */
void queues_pause_on(void);

/**
 * Unpause (run) queue-management of dunst
 *
 * @post Calling update_lists() is neccessary
 */
void queues_pause_off(void);

/**
 * Get the current status
 *
 * @return true if paused
 * @return false if running
 */
bool queues_pause_status(void);

/**
 * Remove all notifications from all list and free the notifications
 *
 * @pre At least one time queues_init() called
 */
void teardown_queues(void);

#endif
/* vim: set tabstop=8 shiftwidth=8 expandtab textwidth=0: */
