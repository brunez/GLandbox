import os
import sys
        
def scanAndAdd(dir, notice):
    for root, subdirs, files in os.walk(dir):
        print('--\nroot = ' + root)
        #list_file_path = os.path.join(root, 'my-directory-list.txt')
        #print('list_file_path = ' + list_file_path)
        for subdir in subdirs:
            print('\t- subdirectory ' + subdir)
            scanAndAdd(subdir, notice)

        for filename in files:
            file_path = os.path.join(root, filename)

            file = open(file_path, 'rb')
            try:                
                content = file.read()
                if not notice in content:
                    content = notice + content
                    dump = open(file_path, 'wb')
                    try:
                        dump.write(content)
                    
                    finally:
                        dump.close()
            finally:
                file.close()
                print('\t- file %s (full path: %s)' % (filename, file_path))


walk_dir = sys.argv[1]

print('walk_dir = ' + walk_dir)

# If your current working directory may change during script execution, it's recommended to
# immediately convert program arguments to an absolute path. Then the variable root below will
# be an absolute path as well. Example:
# walk_dir = os.path.abspath(walk_dir)
print('walk_dir (absolute) = ' + os.path.abspath(walk_dir))

notice = "/***************************************************************************"+"\n*"+"\n* Copyright 2014 Bruno Ordozgoiti"+"\n* Distributed under GNU GPLv3"+"\n*"+"\n* This file is part of GLandbox."+"\n*"+"\n* GLandbox is free software: you can redistribute it and/or modify"+"\n* it under the terms of the GNU General Public License as published by"+"\n* the Free Software Foundation, either version 3 of the License, or"+"\n* (at your option) any later version."+"\n*"+"\n* GLandbox is distributed in the hope that it will be useful,"+"\n* but WITHOUT ANY WARRANTY; without even the implied warranty of"+"\n* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the"+"\n* GNU General Public License for more details."+"\n*"+"\n* You should have received a copy of the GNU General Public License"+"\n* along with GLandbox. If not, see <http://www.gnu.org/licenses/>."+"\n*"+"\n***************************************************************************/\n\n"

scanAndAdd(os.path.abspath(walk_dir), notice)
