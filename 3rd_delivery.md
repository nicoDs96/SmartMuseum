# Third Delivery
This document explains the comments we have received to the second version of the project and how we addressed them.

## Comments 
The main comment we received on the second version of the project was that: it was not clearly stated in the [Evaluation.md](Evaluation.md) file how we wanted to evaluate the project. Then it was also suggested to do not omit actuator simulation but instead to simulate it using a simple relè and a led. The new version of the [Evaluation.md](Evaluation.md) file now provides those informations.  


## Design, Architecture, Evaluation Changes
No big changes were done to our 2nd version of [Design](Design.md),[Architecture](Architecture.md) and [Evaluation](Evaluation.md) documents. The major difference is in the [Evaluation](Evaluation.md) document, where two chapters are added at the end to explain the missing features as described above and a new testing board is added. Indeed, to try to implement a real case scenario we have purchased a [board](https://heltec.org/project/wifi-lora-32/) supporting both lora and wifi (and bluetooth) without any extension, and we tested the code on it. The only drawback is that here we do not have access to real environmental sensor but we emulated them.

## Currently Implemented Functionalities
The PoC currently supports environmental monitoring and emulate actuators to adjust indoor microclimate. In detail we provide:
1. temperature and humidity measurements simulation feature; 
7. actuators (simulated with a simple led).
2. the possibility to send measures to the cloud with LoRaWAN using TheThingsNetwork infrastructure;
3. the possibility to switch to MQTT on-demand and have real-time monitoring and then switch back to LoRaWAN less frequent messages;
4. persistent storage;
5. interactive web dashboard.  
6. adaptive transmission rates.

## Evaluation Results

1.
2.

we have missed to evaluate (and we did not plan to do it for the moment) the overall UX of the museum dashboard and we did not evaluate the capacity of our backend. This is because we think that the amount of traffic will be very low and also because the overall dashboard is far from being complete and presentable to a final user.

