1.17.1 (February 25, 2021)
==========================

Incompatible Behavior Changes
-----------------------------
*Changes that are expected to cause an incompatibility if applicable; deployment changes are likely required*

Minor Behavior Changes
----------------------
*Changes that may cause incompatibilities for some users, but should not for most*

* http: reject requests with #fragment in the URI path. The fragment is not allowed to be part of request
  URI according to RFC3986 (3.5), RFC7230 (5.1) and RFC 7540 (8.1.2.3). Rejection of requests can be changed
  to stripping the #fragment instead by setting the runtime guard `envoy.reloadable_features.http_reject_path_with_fragment`
  to false. This behavior can further be changed to the deprecated behavior of keeping the fragment by setting the runtime guard
  `envoy.reloadable_features.http_strip_fragment_from_path_unsafe_if_disabled`. This runtime guard must only be set
  to false when existing non-compliant traffic relies on #fragment in URI. When this option is enabled, Envoy request
  authorization extensions may be bypassed. This override and its associated behavior will be decommissioned after the standard deprecation period.
* http: when envoy run out of ``max_requests_per_connection``, it will send an HTTP/2 "shutdown nofitication" (GOAWAY frame with max stream ID) and go to a default grace period of 5000 milliseconds (5 seconds) if ``drain_timeout`` is not specified. During this grace period, envoy will continue to accept new streams. After the grace period, a final GOAWAY is sent and envoy will start refusing new streams. However before bugfix, during the grace period, every time a new stream is received, old envoy will always send a "shutdown notification" and restart drain again which actually causes the grace period to be extended and is no longer equal to ``drain_timeout``.

Bug Fixes
---------
*Changes expected to improve the state of the world and are unlikely to have negative effects*

* jwt_authn: reject requests with a proper error if JWT has the wrong issuer when allow_missing is used. Before this change, the requests are accepted.
* overload: fix a bug that can cause use-after-free when one scaled timer disables another one with the same duration.
* ext_authz: fix the ext_authz filter to correctly merge multiple same headers using the ',' as separator in the check request to the external authorization service.
* http: limit use of deferred resets in the http2 codec to server-side connections. Use of deferred reset for client connections can result in incorrect behavior and performance problems.
* jwt_authn: unauthorized responses now correctly include a `www-authenticate` header.
* upstream: cluster slow start config add ``min_weight_percent`` field to avoid too big EDF deadline which cause slow start endpoints receiving no traffic, default 10%. This fix is releted to `issue#19526 <https://github.com/envoyproxy/envoy/issues/19526>`_.

Removed Config or Runtime
-------------------------
*Normally occurs at the end of the* :ref:`deprecation period <deprecated>`

New Features
------------
* http: added the ability to preserve HTTP/1 header case across the proxy. See the :ref:`header casing <config_http_conn_man_header_casing>` documentation for more information.
* http: added support for :ref:`max_requests_per_connection <envoy_v3_api_field_config.core.v3.HttpProtocolOptions.max_requests_per_connection>` for both upstream and downstream connections.
* listener: added an option when balancing across active listeners and wildcard matching is used to return the listener that matches the IP family type associated with the listener's socket address. It is off by default, but is turned on by default in v1.19. To set change the runtime guard `envoy.reloadable_features.listener_wildcard_match_ip_family` to true.
* upstream: added support for :ref:`slow start mode <arch_overview_load_balancing_slow_start>`, which allows to progresively increase traffic for new endpoints.
* upstream: extended :ref:`Round Robin load balancer configuration <envoy_v3_api_field_config.cluster.v3.Cluster.round_robin_lb_config>` with :ref:`slow start <envoy_v3_api_field_config.cluster.v3.Cluster.RoundRobinLbConfig.slow_start_config>` support.
* upstream: extended :ref:`Least Request load balancer configuration <envoy_v3_api_field_config.cluster.v3.Cluster.least_request_lb_config>` with :ref:`slow start <envoy_v3_api_field_config.cluster.v3.Cluster.LeastRequestLbConfig.slow_start_config>` support.


Deprecated
----------
