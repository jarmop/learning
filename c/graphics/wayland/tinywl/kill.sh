#!/bin/bash

ps -A | grep tinywl | kill -15 $(awk '{print $1}')