#ifndef _CONN_FLAGS_H
#define _CONN_FLAGS_H

namespace net {
	enum status_code {
		none = 0,

		// Informational
		i_continue = 100,			// The server has received the request headers and the client should proceed to send the request body (in the case of a request for which a body needs to be sent; for example, a POST request). Sending a large request body to a server after a request has been rejected for inappropriate headers would be inefficient. To have a server check the request's headers, a client must send Expect: 100-continue as a header in its initial request and receive a 100 Continue status code in response before sending the body. The response 417 Expectation Failed indicates the request should not be continued.
		i_switching_rotocols = 101,	// The requester has asked the server to switch protocols and the server has agreed to do so.
		i_processing = 102,			// (WebDAV; RFC 2518) A WebDAV request may contain many sub-requests involving file operations, requiring a long time to complete the request. This code indicates that the server has received and is processing the request, but no response is available yet. This prevents the client from timing out and assuming the request was lost.


		// Success
		s_ok = 200,								// Standard response for successful HTTP requests. The actual response will depend on the request method used. In a GET request, the response will contain an entity corresponding to the requested resource. In a POST request, the response will contain an entity describing or containing the result of the action.
		s_created = 201,						// The request has been fulfilled, resulting in the creation of a new resource.
		s_accepted = 202,						// The request has been accepted for processing, but the processing has not been completed. The request might or might not be eventually acted upon, and may be disallowed when processing occurs.
		s_non_authoritative_information = 203,	// (since HTTP/1.1) The server is a transforming proxy (e.g. a Web accelerator) that received a 200 OK from its origin, but is returning a modified version of the origin's response.
		s_no_content = 204,						// The server successfully processed the request and is not returning any content.
		s_reset_content = 205,					// The server successfully processed the request, but is not returning any content. Unlike a 204 response, this response requires that the requester reset the document view.
		s_partial_content = 206,				// (RFC 7233) The server is delivering only part of the resource (byte serving) due to a range header sent by the client. The range header is used by HTTP clients to enable resuming of interrupted downloads, or split a download into multiple simultaneous streams.
		s_multi_status = 207,					// (WebDAV; RFC 4918) The message body that follows is an XML message and can contain a number of separate response codes, depending on how many sub-requests were made.
		s_already_reported = 208,				// (WebDAV; RFC 5842) The members of a DAV binding have already been enumerated in a previous reply to this request, and are not being included again.
		s_im_used = 226,						// (RFC 3229) The server has fulfilled a request for the resource, and the response is a representation of the result of one or more instance-manipulations applied to the current instance.


		// Redirect
		r_multiple_choices = 300,		// Indicates multiple options for the resource from which the client may choose (via agent-driven content negotiation). For example, this code could be used to present multiple video format options, to list files with different filename extensions, or to suggest word-sense disambiguation.
		r_moved_permanently = 301,		// This and all future requests should be directed to the given URI.
		r_found = 302,					// This is an example of industry practice contradicting the standard. The HTTP/1.0 specification (RFC 1945) required the client to perform a temporary redirect (the original describing phrase was "Moved Temporarily"), but popular browsers implemented 302 with the functionality of a 303 See Other. Therefore, HTTP/1.1 added status codes 303 and 307 to distinguish between the two behaviours. However, some Web applications and frameworks use the 302 status code as if it were the 303.
		r_see_other = 303,				// (since HTTP/1.1) The response to the request can be found under another URI using a GET method. When received in response to a POST (or PUT/DELETE), the client should presume that the server has received the data and should issue a redirect with a separate GET message.
		r_not_modified = 304,			// (RFC 7232) Indicates that the resource has not been modified since the version specified by the request headers If-Modified-Since or If-None-Match. In such case, there is no need to retransmit the resource since the client still has a previously-downloaded copy.
		r_use_proxy = 305,				// (since HTTP/1.1) The requested resource is available only through a proxy, the address for which is provided in the response. Many HTTP clients (such as Mozilla and Internet Explorer) do not correctly handle responses with this status code, primarily for security reasons.
		r_switch_proxy = 306,			// No longer used. Originally meant "Subsequent requests should use the specified proxy."
		r_temporary_redirect = 307,		// (since HTTP/1.1) In this case, the request should be repeated with another URI; however, future requests should still use the original URI. In contrast to how 302 was historically implemented, the request method is not allowed to be changed when reissuing the original request. For example, a POST request should be repeated using another POST request.
		r_permanent_redirect = 308,		// (RFC 7538) The request and all future requests should be repeated using another URI. 307 and 308 parallel the behaviors of 302 and 301, but do not allow the HTTP method to change. So, for example, submitting a form to a permanently redirected resource may continue smoothly.[30


		// Client Error
		ce_bad_request = 400,						// The server cannot or will not process the request due to an apparent client error (e.g., malformed request syntax, too large size, invalid request message framing, or deceptive request routing).
		ce_unauthorized = 401,						// (RFC 7235) Similar to 403 Forbidden, but specifically for use when authentication is required and has failed or has not yet been provided. The response must include a WWW-Authenticate header field containing a challenge applicable to the requested resource. See Basic access authentication and Digest access authentication. 401 semantically means "unauthenticated", i.e. the user does not have the necessary credentials. Note: Some sites issue HTTP 401 when an IP address is banned from the website (usually the website domain) and that specific address is refused permission to access a website.
		ce_payment_required = 402,					// Reserved for future use. The original intention was that this code might be used as part of some form of digital cash or micropayment scheme, but that has not happened, and this code is not usually used. Google Developers API uses this status if a particular developer has exceeded the daily limit on requests.
		ce_forbidden = 403,							// The request was a valid request, but the server is refusing to respond to it. The user might be logged in but does not have the necessary permissions for the resource.
		ce_not_found = 404,							// The requested resource could not be found but may be available in the future. Subsequent requests by the client are permissible.
		ce_method_not_allowed = 405,				// A request method is not supported for the requested resource; for example, a GET request on a form which requires data to be presented via POST, or a PUT request on a read-only resource.
		ce_not_acceptable = 406,					// The requested resource is capable of generating only content not acceptable according to the Accept headers sent in the request. See Content negotiation.
		ce_proxy_authentication_required = 407,		// (RFC 7235) The client must first authenticate itself with the proxy.
		ce_request_time_out = 408,					// The server timed out waiting for the request. According to HTTP specifications: "The client did not produce a request within the time that the server was prepared to wait. The client MAY repeat the request without modifications at any later time."
		ce_conflict = 409,							// Indicates that the request could not be processed because of conflict in the request, such as an edit conflict between multiple simultaneous updates.
		ce_gone = 410,								// Indicates that the resource requested is no longer available and will not be available again. This should be used when a resource has been intentionally removed and the resource should be purged. Upon receiving a 410 status code, the client should not request the resource in the future. Clients such as search engines should remove the resource from their indices. Most use cases do not require clients and search engines to purge the resource, and a "404 Not Found" may be used instead.
		ce_length_required = 411,					// The request did not specify the length of its content, which is required by the requested resource.
		ce_precondition_failed = 412,				// (RFC 7232) The server does not meet one of the preconditions that the requester put on the request.
		ce_payload_too_large = 413,					// (RFC 7231) The request is larger than the server is willing or able to process. Previously called "Request Entity Too Large".
		ce_uri_too_long = 414,						// (RFC 7231) The URI provided was too long for the server to process. Often the result of too much data being encoded as a query-string of a GET request, in which case it should be converted to a POST request. Called "Request-URI Too Long" previously.
		ce_unsupported_media_type = 415,			// The request entity has a media type which the server or resource does not support. For example, the client uploads an image as image/svg+xml, but the server requires that images use a different format.
		ce_range_not_satisfiable = 416,				// (RFC 7233) The client has asked for a portion of the file (byte serving), but the server cannot supply that portion. For example, if the client asked for a part of the file that lies beyond the end of the file. Called "Requested Range Not Satisfiable" previously.
		ce_expectation_failed = 417,				// The server cannot meet the requirements of the Expect request-header field.
		ce_im_a_teapot = 418,						// (RFC 2324) This code was defined in 1998 as one of the traditional IETF April Fools' jokes, in RFC 2324, Hyper Text Coffee Pot Control Protocol, and is not expected to be implemented by actual HTTP servers. The RFC specifies this code should be returned by teapots requested to brew coffee. This HTTP status is used as an easter egg in some websites, including Google.com.
		ce_misdirected_request = 421,				// (RFC 7540) The request was directed at a server that is not able to produce a response (for example because a connection reuse).
		ce_unprocessable_entity = 422,				// (WebDAV; RFC 4918) The request was well-formed but was unable to be followed due to semantic errors.
		ce_locked = 423,							// (WebDAV; RFC 4918) The resource that is being accessed is locked.
		ce_failed_dependency = 424,					// (WebDAV; RFC 4918) The request failed due to failure of a previous request (e.g., a PROPPATCH).
		ce_upgrade_required = 426,					// The client should switch to a different protocol such as TLS/1.0, given in the Upgrade header field.
		ce_precondition_required = 428,				// (RFC 6585) The origin server requires the request to be conditional. Intended to prevent "the 'lost update' problem, where a client GETs a resource's state, modifies it, and PUTs it back to the server, when meanwhile a third party has modified the state on the server, leading to a conflict."
		ce_too_many_requests = 429,					// (RFC 6585) The user has sent too many requests in a given amount of time. Intended for use with rate-limiting schemes.
		ce_request_header_fields_too_large = 431,	// (RFC 6585) The server is unwilling to process the request because either an individual header field, or all the header fields collectively, are too large.
		ce_unavailable_for_legal_reasons = 451,		// A server operator has received a legal demand to deny access to a resource or to a set of resources that includes the requested resource. The code 451 was chosen as a reference to the novel Fahrenheit 451.


		// Server Error
		se_internal_server_error = 500,				// A generic error message, given when an unexpected condition was encountered and no more specific message is suitable.
		se_not_implemented = 501,					// The server either does not recognize the request method, or it lacks the ability to fulfill the request. Usually this implies future availability (e.g., a new feature of a web-service API).[citation needed]
		se_bad_gateway = 502,						// The server was acting as a gateway or proxy and received an invalid response from the upstream server.
		se_service_unavailable = 503,				// The server is currently unavailable (because it is overloaded or down for maintenance). Generally, this is a temporary state.
		se_gateway_time_out = 504,					// The server was acting as a gateway or proxy and did not receive a timely response from the upstream server.
		se_http_version_not_supported = 505,		// The server does not support the HTTP protocol version used in the request.
		se_variant_also_negotiates = 506,			// (RFC 2295) Transparent content negotiation for the request results in a circular reference.
		se_insufficient_storage = 507,				// (WebDAV; RFC 4918) The server is unable to store the representation needed to complete the request.
		se_loop_detected = 508,						// (WebDAV; RFC 5842) The server detected an infinite loop while processing the request (sent in lieu of 208 Already Reported).
		se_not_extended = 510,						// (RFC 2774) Further extensions to the request are required for the server to fulfill it.
		se_network_authentication_required = 511	// (RFC 6585) The client needs to authenticate to gain network access. Intended for use by intercepting proxies used to control access to the network (e.g., "captive portals" used to require agreement to Terms of Service before granting full Internet access via a Wi-Fi hotspot).[
	};

	/*True if status is an informational code; otherwise false*/
	inline bool status_informational(const status_code & status) {
		return status >= i_continue && status <= i_processing;
	}
	/*True if status is a success code; otherwise false*/
	inline bool status_success(const status_code & status) {
		return status >= s_ok && status <= s_im_used;
	}
	/*True if status is a redirect code; otherwise false*/
	inline bool status_redirect(const status_code & status) {
		return status >= r_multiple_choices && status <= r_permanent_redirect;
	}
	/*True if status is a client error code; otherwise false*/
	inline bool status_client_error(const status_code & status) {
		return status >= ce_bad_request && status <= ce_unavailable_for_legal_reasons;
	}
	/*True if status is a server error code; otherwise false*/
	inline bool status_server_error(const status_code & status) {
		return status >= se_internal_server_error && status <= se_network_authentication_required;
	}
}
#endif
