from subprocess import Popen, PIPE, STDOUT


# Execute a command, and ensure a print of the shell output.
def ShellExecute(cmd, bSilent=False):
    p = Popen(cmd, shell=True, stdin=PIPE, stdout=PIPE, stderr=STDOUT)
    output = p.stdout.read()
    if (not bSilent) and output:
        print(output)


ShellExecute('"Premake5.exe" --help')
input('')

