# Hello World example for the uVisor

This is a Hello World yotta executable created to show some of the security features provided by the uVisor in mbed. The application hosts a secure box that is able to generate a secret at runtime. The secret is stored in the box context, so it is protected by uVisor against the rest of the code.

This is a challenge. For the security model to be effective, the box secret must not be leaked in any way. You can attempt whatever approach you prefer to try and read the secret. By default, the main event in this application continuously tries to verify its challenge against the box secret.
