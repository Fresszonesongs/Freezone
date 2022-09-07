# -*- coding: utf-8 -*-

import subprocess
import logging
import os
import time
import sys
import json
import datetime

from .freezone_tools import *

LOG_LEVEL = logging.INFO
LOG_FORMAT = "%(asctime)-15s - %(name)s - %(levelname)s - %(message)s"
MAIN_LOG_PATH = "./freezone_runner.log"

MODULE_NAME = "SPS-Tester-via-freezonepy.Runner-Py"
logger = logging.getLogger(MODULE_NAME)
logger.setLevel(LOG_LEVEL)


class freezoneNode(object):
    def __init__(self, freezone_executable, working_dir, config_src_path):
        self.freezone_executable = freezone_executable
        self.working_dir = working_dir
        self.config_src_path = config_src_path

        from shutil import rmtree, copy
        # remove old data from node
        if os.path.exists(self.working_dir):
            rmtree(self.working_dir)
        os.makedirs(self.working_dir+"/blockchain")
        # copy config file to working dir
        copy(self.config_src_path, self.working_dir + "/config.ini")

        self.freezone_config = self.parse_node_config_file(self.working_dir + "/config.ini")
        self.ip_address, self.port = self.freezone_config["webserver-http-endpoint"][0].split(":")
        self.ip_address = "http://{}".format(self.ip_address)
        self.node_running = False

    def get_from_config(self, key):
        return self.freezone_config.get(key, None)

    def get_node_url(self):
        return "{}:{}/".format(self.ip_address, self.port)

    def is_running(self):
        return self.node_running

    def parse_node_config_file(self, config_file_name):
        ret = dict()
        lines = None
        with open(config_file_name, "r") as f:
            lines = f.readlines()

        for line in lines:
            proc_line = line.strip()
            if proc_line:
                if proc_line.startswith("#"):
                    continue
                k, v = proc_line.split("=", 1)
                k = k.strip()
                v = v.strip()
                if k in ret:
                    ret[k].append(v)
                else:
                    ret[k] = [v]
        return ret

    def run_freezone_node(self, additional_params = [], wait_for_blocks = True):
        detect_process_by_name("freezone", self.ip_address, self.port)

        logger.info("*** START NODE at {0}:{1} in {2}".format(self.ip_address, self.port, self.working_dir))

        parameters = [
            self.freezone_executable,
            "-d",
            self.working_dir,
            "--advanced-benchmark",
            "--sps-remove-threshold",
            "-1"
        ]

        parameters = parameters + additional_params
        
        self.pid_file_name = "{0}/run_freezone-{1}.pid".format(self.working_dir, self.port)
        current_time_str = datetime.datetime.now().strftime("%Y-%m-%d")
        log_file_name = "{0}/{1}-{2}-{3}.log".format(self.working_dir, "freezone", self.port, current_time_str)
        screen_cfg_name = "{0}/freezone_screen-{1}.cfg".format(self.working_dir, self.port)

        save_screen_cfg(screen_cfg_name, log_file_name)
        screen_params = [
            "screen",
            "-m",
            "-d",
            "-L",
            "-c",
            screen_cfg_name,
            "-S",
            "{0}-{1}-{2}".format("freezone", self.port, current_time_str)
        ]

        parameters = screen_params + parameters
        logger.info("Running freezoned with command: {0}".format(" ".join(parameters)))
        
        try:
            subprocess.Popen(parameters)
            save_pid_file(self.pid_file_name, "freezone", self.port, current_time_str)
            if wait_for_blocks:
                wait_for_blocks_produced(2, "{}:{}".format(self.ip_address, self.port))
            else:
                wait_for_string_in_file(log_file_name, "start listening for ws requests", 240.)
            self.node_running = True
            logger.info("Node at {0}:{1} in {2} is up and running...".format(self.ip_address, self.port, self.working_dir))
        except Exception as ex:
            logger.error("Exception during freezoned run: {0}".format(ex))
            kill_process(self.pid_file_name, "freezone", self.ip_address, self.port)
            self.node_running = False


    def stop_freezone_node(self):
        logger.info("Stopping node at {0}:{1}".format(self.ip_address, self.port))
        kill_process(self.pid_file_name, "freezone", self.ip_address, self.port)
        self.node_running = False


if __name__ == "__main__":
    node = freezoneNode("/home/dariusz-work/Builds/freezone/programs/freezoned/freezoned", "/home/dariusz-work/freezone-data", "./resources/config.ini.in")
    node.run_freezone_node()
    node.stop_freezone_node()
