/*
 * Copyright (c) 2012, Nasel
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef TINS_IEEE802_3_H
#define TINS_IEEE802_3_H

#include <stdint.h>

#include "pdu.h"
#include "endianness.h"
#include "hw_address.h"
#include "network_interface.h"

namespace Tins {

    /** 
     * \brief Class representing an Ethernet II PDU.
     */
    class IEEE802_3 : public PDU {
    public:
        /**
         * \brief The address type.
         */
        typedef HWAddress<6> address_type; 
        
        /**
         * \brief This PDU's flag.
         */
        static const PDU::PDUType pdu_flag = PDU::IEEE802_3;

        /**
         * \brief Represents the IEEE802_3 broadcast address.
         */
        static const address_type BROADCAST;

        /**
         * \brief Constructor for creating an IEEE802_3 PDU
         *
         * Constructor that builds an IEEE802_3 PDU taking the interface name,
         * destination's and source's MAC.
         *
         * \param iface string containing the interface's name from where to send the packet.
         * \param dst_hw_addr The destination hardware address.
         * \param src_hw_addr The source hardware address.
         * \param child The PDU which will be set as the inner PDU.
         */
        IEEE802_3(const NetworkInterface& iface = NetworkInterface(), 
                  const address_type &dst_hw_addr = address_type(), 
                  const address_type &src_hw_addr = address_type(), 
                  PDU* child = 0);

        /**
         * \brief Constructor which creates an IEEE802_3 object from a buffer and adds all identifiable
         * PDUs found in the buffer as children of this one.
         * \param buffer The buffer from which this PDU will be constructed.
         * \param total_sz The total size of the buffer.
         */
        IEEE802_3(const uint8_t *buffer, uint32_t total_sz);

        /* Getters */
        /**
         * \brief Getter for the destination hardware address.
         *
         * \return The destination hardware address.
         */
        address_type dst_addr() const { return _eth.dst_mac; }

        /**
         * \brief Getter for the source hardware address.
         *
         * \return The source hardware address.
         */
        address_type src_addr() const { return _eth.src_mac; }

        /**
         * \brief Getter for the interface.
         *
         * \return The network interface.
         */
        const NetworkInterface &iface() const { return this->_iface; }

        /**
         * \brief Getter for the length field.
         * \return The length field value.
         */
        uint16_t length() const { return Endian::be_to_host(_eth.length); };

        /* Setters */

        /**
         * \brief Setter for the destination hardware address.
         *
         * \param new_dst_mac The new destination hardware address.
         */
        void dst_addr(const address_type &new_dst_mac);

        /**
         * \brief Setter for the source hardware address.
         *
         * \param new_src_mac The new source hardware address.
         */
        void src_addr(const address_type &new_src_mac);

        /**
         * \brief Setter for the interface.
         *
         * \param new_iface The interface in which to send this PDU.
         */
        void iface(const NetworkInterface &new_iface);

        /**
         * \brief Setter for the length field.
         *
         * \param new_length uint16_t with the new value of the length field.
         */
        void length(uint16_t new_length);

        /* Virtual methods */
        /**
         * \brief Returns the IEEE802_3 frame's header length.
         *
         * \return An uint32_t with the header's size.
         * \sa PDU::header_size()
         */
        uint32_t header_size() const;
        
        #ifndef WIN32
        /**
         * \sa PDU::send()
         */
        void send(PacketSender &sender);
        #endif // WIN32

        /** \brief Check wether ptr points to a valid response for this PDU.
         *
         * \sa PDU::matches_response
         * \param ptr The pointer to the buffer.
         * \param total_sz The size of the buffer.
         */
        bool matches_response(uint8_t *ptr, uint32_t total_sz);

        #ifndef WIN32
        /** \brief Receives a matching response for this packet.
         *
         * \sa PDU::recv_response
         * \param sender The packet sender which will receive the packet.
         */
        PDU *recv_response(PacketSender &sender);
        #endif // WIN32

        /**
         * \brief Getter for the PDU's type.
         * \sa PDU::pdu_type
         */
        PDUType pdu_type() const { return PDU::IEEE802_3; }

        /** 
         * \brief Clones this pdu, filling the corresponding header with data
         * extracted from a buffer.
         *
         * \param ptr The pointer to the from from which the data will be extracted.
         * \param total_sz The size of the buffer.
         * \return The cloned PDU.
         * \sa PDU::clone_packet
         */
        PDU *clone_packet(const uint8_t *ptr, uint32_t total_sz);

        /**
         * \sa PDU::clone
         */
        IEEE802_3 *clone() const {
            return new IEEE802_3(*this);
        }
    private:
        /**
         * Struct that represents the Ethernet II header
         */
        struct ethhdr {
            uint8_t dst_mac[address_type::address_size];
            uint8_t src_mac[address_type::address_size];
            uint16_t length;
        } __attribute__((__packed__));

        void write_serialization(uint8_t *buffer, uint32_t total_sz, const PDU *parent);


        ethhdr _eth;
        NetworkInterface _iface;
    };

};


#endif // TINS_IEEE802_3_H
