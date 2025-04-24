/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EventEncoder.h"

#ifndef EXCLUDE_EVENT_ENCODER

#ifdef FUNCTIONAL_SUPPORTED
    #include <functional>
#endif

/**
 * Construct a rotary encoder
 */
// EventEncoder::EventEncoder(uint8_t pin1, uint8_t pin2) 
//     : encoderPin1(pin1), encoderPin2(pin2) { }

EventEncoder::EventEncoder(EncoderAdapter *encoderAdapter) {
    encoder = encoderAdapter;
}

EventEncoder::~EventEncoder() {
    delete encoder;
}

void EventEncoder::begin() {
    encoder->begin(); // = new Encoder(encoderPin1, encoderPin2); 
}

void EventEncoder::invoke(InputEventType et) {
    if ( isInvokable(et) ) {
        callbackFunction(et, *this);
    }    
}


void EventEncoder::onEnabled() {
    //Reset the encoder so we don't trigger other events
    //idleFlagged = true;
    //encoder->write(encoderPosition*positionDivider);
    encoder->setPosition(currentPosition*positionDivider);
    invoke(InputEventType::ENABLED);
}

void EventEncoder::unsetCallback() {
    callbackFunction = nullptr;
    EventInputBase::unsetCallback();
}

void EventEncoder::update() {
    // @TODO Do we store the current position when disabled and update if re-enabled?
    if ( _enabled ) {
        //encoder udate (fires encoder rotation callbacks)
        if ( millis() > (rateLimitCounter + rateLimit) ) { 
            readIncrement();
            if ( encoderIncrement !=0 ) {
                currentPosition += encoderIncrement;
                invoke(InputEventType::CHANGED);
            }
            rateLimitCounter = millis();
        }
        EventInputBase::update();
    }
}


// void EventEncoder::readIncrement() {
//     #warning "readIncrement rounding"
//     long newPosition = round(encoder->getPosition()/positionDivider);
//     encoderIncrement = newPosition - oldPosition;
//     oldPosition = newPosition;
// }

/*
21:14:06:148 -> Counts: 4 Position: 1
21:14:07:204 -> Counts: 8 Position: 2
21:14:07:942 -> Counts: 7 Position: 1
21:14:10:200 -> Counts: 3 Position: 0
21:14:11:552 -> Counts: -4 Position: -1
21:14:12:602 -> Counts: -3 Position: 0
21:14:13:347 -> Counts: 4 Position: 1
21:14:14:447 -> Counts: 8 Position: 2
21:14:15:510 -> Counts reset
21:14:15:511 -> Counts: 0 Position: 0
21:14:16:588 -> Counts: 4 Position: 1
21:14:18:312 -> Counts reset
21:14:18:312 -> Counts: 0 Position: 0
21:14:18:855 -> Counts: -4 Position: -1
21:14:19:786 -> Counts: -8 Position: -2
21:14:20:545 -> Counts: -12 Position: -3
21:14:21:101 -> Counts: -16 Position: -4
21:14:21:756 -> Counts: -20 Position: -5
21:14:22:969 -> Counts: -24 Position: -6
21:14:24:552 -> Counts: -28 Position: -7
21:14:25:699 -> Counts: -32 Position: -8
21:14:27:024 -> Counts: -31 Position: -7
21:14:29:786 -> Counts: -27 Position: -6
21:14:31:305 -> Counts: -23 Position: -5
21:14:32:561 -> Counts: -19 Position: -4
21:14:33:574 -> Counts: -15 Position: -3
21:14:43:575 -> Counts: -12 Position: -3
*/

void EventEncoder::readIncrement() {
    #warning "readIncrement remainder"
    encoderIncrement = 0;
    long newPosition = encoder->getPosition();

    counts = newPosition;
    if (counts != oldCounts) {
        Serial.print("Counts: "); 
        Serial.print(newPosition); 
        Serial.print(" Position: ");     
       
    }
    
    if ( newPosition % positionDivider == 0 ) {
        newPosition = newPosition/positionDivider;
        encoderIncrement = newPosition - oldPosition;
        oldPosition = newPosition;
    }

    if (counts != oldCounts) {
        Serial.println(oldPosition);
        oldCounts = counts;   
    }
}
    
    

// void EventEncoder::readIncrement() {
//     long newPosition = floor(encoder->getPosition()/positionDivider);     
//     encoderIncrement = newPosition - oldPosition;
//     oldPosition = newPosition;
// }


// void EventEncoder::readIncrement() {
//     // long newPosition = floor(encoder->getPosition()/positionDivider);
//     long newPosition = encoder->getPosition()/positionDivider;
//     if ( newPosition > oldPosition ) { newPosition = floor(newPosition); }
//     else if ( newPosition < oldPosition ) { newPosition = ceil(newPosition); }
//     encoderIncrement = newPosition - oldPosition;
//     oldPosition = newPosition;
// }
// void EventEncoder::readIncrement() {
//     counts = encoder->getPosition();
//     long newPosition = 0;
//     if ( counts > oldCounts ) {
//         if (counts == ( oldCounts + positionDivider )) {
//             newPosition = counts / positionDivider;
//             oldCounts = counts;
//             oldPosition = newPosition;
//             encoderIncrement = newPosition - oldPosition;   
//         }
//     }
//     else if ( counts < oldCounts ) {
//         if (counts == ( oldCounts - positionDivider )) {
//             newPosition = counts / positionDivider;
//             oldCounts = counts;
//             oldPosition = newPosition;
//             encoderIncrement = newPosition - oldPosition;   
//         }
//     }
     
// }

#else 

#pragma message("Info: EventEncoder and EventEncoderButton excluded from your build. Install the EncoderAdapter library, an Encoder library and #include the required adapter class (eg <PjrcEncoderAdapter.h>) to resolve.")

#endif