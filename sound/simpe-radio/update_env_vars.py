import os
Import("env")

env["LOCAL_SSID"] = os.environ['LOCAL_SSID']
env["LOCAL_PASSWORD"] = os.environ['LOCAL_PASSWORD']