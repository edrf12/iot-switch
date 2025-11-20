#include "remote.h"

AC::AC(const char* id, const char* name, uint8_t ir_pin, DHT* dht)
    : entity(id, HAHVAC::TargetTemperatureFeature | HAHVAC::PowerFeature |
                     HAHVAC::ModesFeature | HAHVAC::FanFeature),
      sender(ir_pin),
      dht(dht) {
    entity.setName(name);
}

stdAc::opmode_t AC::convert_mode(HAHVAC::Mode mode) {
    switch (mode) {
        case HAHVAC::AutoMode:
            return stdAc::opmode_t::kAuto;
        case HAHVAC::CoolMode:
            return stdAc::opmode_t::kCool;
        case HAHVAC::DryMode:
            return stdAc::opmode_t::kDry;
        case HAHVAC::FanOnlyMode:
            return stdAc::opmode_t::kFan;
        default:
            return stdAc::opmode_t::kAuto;
    }
}

stdAc::fanspeed_t AC::convert_fan(HAHVAC::FanMode mode) {
    switch (mode) {
        case HAHVAC::AutoFanMode:
            return stdAc::fanspeed_t::kAuto;
        case HAHVAC::LowFanMode:
            return stdAc::fanspeed_t::kLow;
        case HAHVAC::MediumFanMode:
            return stdAc::fanspeed_t::kMedium;
        case HAHVAC::HighFanMode:
            return stdAc::fanspeed_t::kHigh;
        default:
            return stdAc::fanspeed_t::kAuto;
    }
}

void AC::onPowerCommand(bool state, HAHVAC* sender) {
    if (state) {
        this->sender.next.power = true;
    } else {
        this->sender.next.power = false;
    }
    this->sender.sendAc();
}

void AC::onModeCommand(HAHVAC::Mode mode, HAHVAC* sender) {
    if (mode == HAHVAC::OffMode) {
        this->sender.next.power = false;
        this->sender.next.mode = stdAc::opmode_t::kOff;
    } else {
        this->sender.next.power = true;
        this->sender.next.mode = convert_mode(mode);
    }
    this->sender.sendAc();
    sender->setMode(mode);
}

void AC::onFanModeCommand(HAHVAC::FanMode mode, HAHVAC* sender) {
    this->sender.next.fanspeed = convert_fan(mode);
    this->sender.sendAc();
    sender->setFanMode(mode);
}

void AC::onTargetTemperatureCommand(HANumeric target, HAHVAC* sender) {
    this->sender.next.degrees = target.toFloat();
    this->sender.sendAc();
    sender->setTargetTemperature(target);
}

void AC::setup() {
    // Setup sender
    this->sender.next.protocol = decode_type_t::FUJITSU_AC;
    this->sender.next.model = 1;
    this->sender.next.celsius = true;
    this->sender.next.swingv = stdAc::swingv_t::kOff;
    this->sender.next.swingh = stdAc::swingh_t::kOff;
    this->sender.next.light = false;
    this->sender.next.beep = false;
    this->sender.next.econo = false;
    this->sender.next.filter = false;
    this->sender.next.turbo = false;
    this->sender.next.quiet = false;
    this->sender.next.sleep = -1;
    this->sender.next.clean = false;
    this->sender.next.clock = -1;

    // Setup HomeAssistant entity (if needed)
    this->entity.setMinTemp(18);
    this->entity.setMaxTemp(30);
    this->entity.setTempStep(1);
    this->entity.setModes(HAHVAC::AutoMode | HAHVAC::OffMode |
                          HAHVAC::CoolMode | HAHVAC::FanOnlyMode |
                          HAHVAC::DryMode);
    this->entity.setCurrentTemperature(this->room_temperature);

    if (this->entity.getCurrentTargetTemperature().isSet()) {
        this->sender.next.degrees =
            this->entity.getCurrentTargetTemperature().toFloat();
    } else {
        float temp = 24;
        this->entity.setCurrentTargetTemperature(temp);
        this->sender.next.degrees = temp;
    }
    this->sender.next.mode = convert_mode(this->entity.getCurrentMode());
    this->sender.next.fanspeed = convert_fan(this->entity.getCurrentFanMode());

    this->sender.next.power =
        this->entity.getCurrentMode() == HAHVAC::Mode::OffMode ? false : true;

    // Bind to events
    entity.onPowerCommand([this](bool state, HAHVAC* sender) {
        this->onPowerCommand(state, sender);
    });

    entity.onModeCommand([this](HAHVAC::Mode mode, HAHVAC* sender) {
        this->onModeCommand(mode, sender);
    });

    entity.onFanModeCommand([this](HAHVAC::FanMode mode, HAHVAC* sender) {
        this->onFanModeCommand(mode, sender);
    });

    entity.onTargetTemperatureCommand(
        [this](HANumeric temperature, HAHVAC* sender) {
            this->onTargetTemperatureCommand(temperature, sender);
        });
}

void AC::loop() {
    if (millis() - this->last_temp_publish > 30000) {
        room_temperature = dht->readTemperature();
        this->entity.setCurrentTemperature(room_temperature);
        this->last_temp_publish += 30000;
    }
}

float AC::getTarget() {
    return this->entity.getCurrentTargetTemperature().toFloat();
}