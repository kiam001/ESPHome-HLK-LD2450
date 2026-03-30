#pragma once
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome::ld2450
{
    /**
     * @brief Simple polling sensor which publishes its values on a regular basis.
     */
    class PollingSensor : public sensor::Sensor, public PollingComponent
    {
    public:
        void setup() override
        {
            // Wir nutzen _ref() um die Warnung zu vermeiden und Performance zu verbessern
            auto &unit = this->get_unit_of_measurement_ref();

            if (unit == "m") 
            {
                conversion_factor_ = 0.001f; // mm zu m
            }
            else if (unit == "cm")
            {
                conversion_factor_ = 0.1f; // mm zu cm
            }
            else 
            {
                conversion_factor_ = 1.0f; // Standard: mm
            }
        }

        void update() override
        {
            // Wenn der Wert sich geändert hat, veröffentlichen
            if (this->state != value_) {
                this->publish_state(value_);
            }
        }

        /**
         * Setzt den neuen Wert und wendet den Umrechnungsfaktor an.
         * @param new_value Wert in mm vom Sensor
         */
        void set_value(float new_value)
        {
            value_ = new_value * conversion_factor_;
        }

    private:
        /// @brief Umrechnungsfaktor (wird in setup() bestimmt)
        float conversion_factor_ = 1.0f;

        /// @brief Aktueller Wert des Sensors
        float value_ = 0.0f;
    };
} // namespace esphome::ld2450
