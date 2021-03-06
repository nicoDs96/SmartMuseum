# Second Delivery
This document explain the criticism we received to the first version of the project and how we addressed them.

## Comments and Critics
The main comment we received on the first version of the project were:
1. Good the idea of a specific target. 
2. The feature for this target (Curators) is pretty straightforward and not related to the main features for users.
3. BluetoothLowEnergy is too common among projects.
4. Monitoring the environment is only a corner feature while it might be the core.
5. No need for AR (Augmented Reality).
6. Good architecture considerations, but we should focus more on the ones specifically related to the IoT.
7. Excellent the comparison of IoT cloud services.  
  
We have decided to address them as follows:
1. The target still remains specific (museum curators) but now visitors are not involved.
2. We only focus on Curators and we try to provide useful and a bit more complex features.
3. We removed it since now it does not fit our need anymore.
4. We made a pivot on the project and we made it the core feature.
5. Removed, it was just speculation on possible feature developments.
6. Now we focused on the IoT components, see [Architecture.md](Architecture.md)
7. Same as the old one but with different parameters to reflect project changes.

## New Idea (Pivot)
Briefly, this project now aims to become a first step for the museum through a transition into intelligent and efficient management of its resources involving two strategic areas: asset preservation and energy efficiency. Indeed, artworks conservation is a key need for a museum as pointed out during the lecture with Sapienza Museum Experts. In the meanwhile, this activity can be performed in a very smart way with the help of IoT, sensors, actuators, to reduce energy cost and to augment human awareness of the process thank to newest technologies.

## Design, Architecture, Evaluation Changes
Since the target became only the Heritage Curators, now all our personas reflect those changes. The storyboard also represent the new features of the project.   
The architecture in its cloud composition does not changes much. Indeed we only removed one service not required anymore. Since we focus only on environmental monitoring, we decided to add on-demand support for MQTT, so now the boards can talk to TheThingsNetwork (TTN) in normal condition through LoRaWAN and can switch to MQTT using AWS IoT when the Curator need a more accurate view.   
The evaluation document reflects the changes in the data model and in the data volume of the application. However the methodology and parameters remain the same. Some User Experience evaluation metric is added: the Think Aloud interview. Also we have analyzed the competitors and we made a brief comparison of features offered. A summary of the hardware is provided as well as a cost evaluation. 

## Currently Implemented Functionalities
At the moment it has been developed the museum API, able to talk with AWS IoT Broker and TTN, while DB interaction is a work in progress. The dashboard is available as a mockup prototype. The Nucleo sensor reading part is ready too, the communication process among boards and broker is work in progress. Actuators will be simulated since the impossibility to access to Museum spaces and know the available resources.

## Through the 3rd Delivery
At the end the PoC will fully support environmental monitoring and only emulate actuators to adjust indoor microclimate. In detail we will provide:
1. temperature, humidity and air pressure measurements feature; 
2. the possibility to send measures to the cloud with LoRaWAN using TheThingsNetwork infrastructure;
3. the possibility to switch to MQTT on-demand and have real time monitoring and then switch back to LoRaWAN less frequent messages;
4. persistent storage;
5. interactive web dashboard.  
At the end we expect also to provide detailed description of network latency in realtime mode and we aim to adjust the transmission rate according to the measured latency. Indeed if we need to measure each 0.05 seconds but the messages takes 1.5 seconds to arrive, the best solution might be to send one message each 1.5 seconds with a list of measurements at frequency of 0.05 seconds as payload.  
We expect also to provide some energy consumption statistics of our hardware.
