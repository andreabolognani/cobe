#ifndef __defined_esc_h
#define __defined_esc_h
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct esc * esc_t;

/** Send the given buffer as escaped message.
 *
 * The buffer is considered as a single message.
 *
 * @param[in] fd The file descriptor to send the message through;
 * @param[in] buffer The buffer containing the message to be sent;
 * @param[in] size The size of the message in bytes.
 *
 * @return >=0 on success, < 0 on failure.
 */
ssize_t esc_send (int fd, const void *buffer, size_t size);

/** Receive an escaped message from a file descriptor
 *
 * This function is provided with a size argument. If the incoming message
 * has a greater length, the message is discarded and -2 is returned.
 *
 * @param[in] fd The file descriptor to read the message from;
 * @param[out] buffer The buffer in which the message will be stored;
 * @param[in] size The available space in the buffer, measured in bytes.
 *
 * @retval -1 on error;
 * @retval -2 if the buffer size is not enough for the incoming message.
 * @return The length of the received message.
 */
ssize_t esc_recv (int fd, void *buffer, size_t size);

#ifdef __cplusplus
}
#endif
#endif // __defined_esc_h

