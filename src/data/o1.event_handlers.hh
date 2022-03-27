/**
 * BSD 3-Clause License
 *
 * Copyright (c) 2021, Gonzalo Arana
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef O1CPPLIB_O1_EVENT_HANDLERS_HH
#define O1CPPLIB_O1_EVENT_HANDLERS_HH

namespace o1 {

	template <typename node_t>
	class NodeEventHandlers {
	public:
		virtual void attaching(node_t*) = 0;
		virtual void attached(node_t*) = 0;
		virtual void detaching(node_t*) = 0;
		virtual void detached(node_t*) = 0;
	};


	template <
		typename base_node_t,
		typename derived_node_t
	>
	class AdaptedNodeEventHandlers:
		public NodeEventHandlers<base_node_t> {
	public:
		virtual void attaching(derived_node_t*) = 0;
		virtual void attached(derived_node_t*) = 0;
		virtual void detaching(derived_node_t*) = 0;
		virtual void detached(derived_node_t*) = 0;

		void
		attaching(base_node_t* node) override {
			attaching(dynamic_cast<derived_node_t*>(node));
		}

		void attached(base_node_t* node) override {
			attached(dynamic_cast<derived_node_t*>(node));
		}

		void
		detaching(base_node_t* node) override {
			detaching(dynamic_cast<derived_node_t*>(node));
		}

		void detached(base_node_t* node) override {
			detached(dynamic_cast<derived_node_t*>(node));
		}
	};


	template <typename node_t, typename container_t>
	class ContainerEventHandlers {
	public:
		virtual void attaching(node_t*, container_t*) = 0;
		virtual void attached(node_t*, container_t*) = 0;
		virtual void detaching(node_t*, container_t*) = 0;
		virtual void detached(node_t*, container_t*) = 0;
	};

	template <
	    typename base_node_t,
		typename base_container_t,
	    typename derived_node_t,
		typename derived_container_t
		>
	class AdaptedContainerEventHandlers:
		public ContainerEventHandlers<base_node_t, base_container_t> {
	public:
		virtual void attaching(derived_node_t*, derived_container_t*) = 0;
		virtual void attached(derived_node_t*, derived_container_t*) = 0;
		virtual void detaching(derived_node_t*, derived_container_t*) = 0;
		virtual void detached(derived_node_t*, derived_container_t*) = 0;

		void
		attaching(base_node_t* node, base_container_t* container) override {
			attaching(
				dynamic_cast<derived_node_t*>(node),
				dynamic_cast<derived_container_t*>(container)
			);
		}

		void attached(base_node_t* node, base_container_t* container) override {
			attached(
				dynamic_cast<derived_node_t*>(node),
				dynamic_cast<derived_container_t*>(container)
			);
		}

		void
		detaching(base_node_t* node, base_container_t* container) override {
			detaching(
				dynamic_cast<derived_node_t*>(node),
				dynamic_cast<derived_container_t*>(container)
			);
		}

		void detached(base_node_t* node, base_container_t* container) override {
			detached(
				dynamic_cast<derived_node_t*>(node),
				dynamic_cast<derived_container_t*>(container)
			);
		}
	};

}

#endif //O1CPPLIB_O1_EVENT_HANDLERS_HH
