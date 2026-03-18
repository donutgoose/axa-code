import ms5837
import time

sensor = ms5837.MS5837_30BA()

if not sensor.init():
    print("Sensor could not be initialized")
    exit(1)

if not sensor.read():
    print("Sensor read failed!") 
    exit(1)

print(("Pressure: %.2f atm %.2f Torr %.2f psi")%(
    sensor.temperature(ms5837.UNITS_Centigrade),
    sensor.temperature(ms5837.UNITS_Farenheit),
    sensor.temperature(ms5837.UNITS_Kelvin)))

freshwaterDepth=sensor.depth()
sensor.setFluidDensity(1000)
print(("Depth: %.3f m(freshwater) %.3f m(saltwater)") % (freshwaterDepth, saltwaterDepth))

print(("MSL Relative Altitude: %.2f m") % sensor.altitude())

time.sleep(3)

while True:
    if sensor.read():
        print((": %0.1f mbar %0.3f psi\tT: %0.2f C %0.2f F") % (
            sensor.pressure(),
            sensor.pressure(ms5837.UNITS_psi),
            sensor.temperature(),
            sensor.temperature(ms5837.UNITS_Farenheit)))
    else:
        print("Sensor read failed!")
        exit(1)
