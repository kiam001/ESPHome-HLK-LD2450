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
            // Prüfen, ob der aktuelle Wert NAN ist
            bool current_is_nan = std::isnan(value_);
            // Prüfen, ob der letzte gesendete Status NAN war
            bool last_was_nan = std::isnan(this->state);

            if (current_is_nan) {
                // Nur senden, wenn wir von "Person da" zu "Person weg" wechseln
                if (!last_was_nan) {
                    this->publish_state(NAN);
                }
            } else {
                // Wenn eine Person da ist (kein NAN):
                // Senden, wenn der letzte Status NAN war ODER wenn sich der Wert geändert hat
                if (last_was_nan || this->state != value_) {
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
