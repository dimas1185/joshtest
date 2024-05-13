#!/bin/bash

curl -d '{"brightness": 0, "id": "61493fb5-e837-43c2-8f25-bac09c35043d", "name": "My new light", "on": true, "room": "Living Room"}' \
     -H "Content-Type: application/json" -X POST http://localhost:8080/lights

sleep 1

curl -X DELETE http://localhost:8080/lights/61493fb5-e837-43c2-8f25-bac09c35043d

sleep 1

curl -d '{"room": "Living Room"}' \
     -H "Content-Type: application/json" -X PUT http://localhost:8080/lights/1b1920d5-22f1-43aa-9d35-371b2075d33d

sleep 1


curl -d '{"name": "Living Room Light"}' \
     -H "Content-Type: application/json" -X PUT http://localhost:8080/lights/1b1920d5-22f1-43aa-9d35-371b2075d33d

sleep 1

curl -d '{"room": "Porch"}' \
     -H "Content-Type: application/json" -X PUT http://localhost:8080/lights/1b1920d5-22f1-43aa-9d35-371b2075d33d

sleep 1

curl -d '{"name": "Porch Light"}' \
     -H "Content-Type: application/json" -X PUT http://localhost:8080/lights/1b1920d5-22f1-43aa-9d35-371b2075d33d

sleep 1


curl -d '{"on": true}' \
     -H "Content-Type: application/json" -X PUT http://localhost:8080/lights/1b1920d5-22f1-43aa-9d35-371b2075d33d

sleep 1

curl -d '{"brightness": 255}' \
     -H "Content-Type: application/json" -X PUT http://localhost:8080/lights/1b1920d5-22f1-43aa-9d35-371b2075d33d

sleep 1

curl -d '{"brightness": 100}' \
     -H "Content-Type: application/json" -X PUT http://localhost:8080/lights/1b1920d5-22f1-43aa-9d35-371b2075d33d

sleep 1

curl -d '{"on": false}' \
     -H "Content-Type: application/json" -X PUT http://localhost:8080/lights/1b1920d5-22f1-43aa-9d35-371b2075d33d

sleep 1