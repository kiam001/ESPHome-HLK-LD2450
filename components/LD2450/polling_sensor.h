#pragma once
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/sensor/sensor.h"
#include <cmath>

namespace esphome::ld2450
{
    /**
     * @brief Polling sensor which only publishes on actual state changes.
     * This version is optimized for ESPHome 2026 and prevents "nan-spamming".
     */
    class PollingSensor : public sensor::Sensor, public PollingComponent
    {
    public:
        void setup() override
        {
            // Einheit bestimmen und Faktor festlegen
            const auto unit = this->get_unit_of_measurement_ref();

            if (unit == "m") 
                conversion_factor_ = 0.001f; // mm -> m
            else if (unit == "cm")
                conversion_factor_ = 0.1f;   // mm -> cm
            else 
                conversion_factor_ = 1.0f;   // Bleibt mm
        }

        void update() override
        {
            // Falls der neue Wert NAN ist (kein Ziel)
            if (std::isnan(value_)) 
            {
                // Nur publizieren, wenn der letzte bekannte Status NICHT NAN war
                if (!std::isnan(this->state)) 
                {
                    this->publish_state(NAN);
                }
            } 
            else 
            {
                // Falls ein echter Wert da ist, nur bei Änderung senden
                if (this->state != value_) 
                {
                    this->publish_state(value_);
                }
            }
        }

        /**
         * Setzt den neuen Rohwert vom Sensor (in mm).
         */
        void set_value(float new_value)
        {
            value_ = new_value * conversion_factor_;
        }

    private:
        float conversion_factor_ = 1.0f;
        float value_ = NAN; // Startwert ist NAN
    };
} // namespace esphome::ld2450
