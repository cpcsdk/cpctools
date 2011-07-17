#!/usr/bin/env python

# -*- coding: utf-8 -*-



"""

stk2bin

(c) SyX, 2009
Distributed under the terms of the MIT Licence.

"""



# Parches para que funcione en python 2.5

from __future__ import with_statement



import sys

import os           # path.exists()

import glob         # glob() expande los patrones de los ficheros

from optparse import make_option, OptionParser



# Procesa la línea de comandos    

def procesar_linea_comandos(linea_de_comandos):

    """

    Devuelve una tupla de dos elementos: (opciones, lista_de_ficheros).

    `linea_de_comandos` es una lista de argumentos, o `None` para ``sys.argv[1:]``.

    """

    if linea_de_comandos is None:

        linea_de_comandos = sys.argv[1:]



    version_programa = "%prog v0.1"

    uso_programa = "usage: %prog [options] song1.sks son2.sks ... songX.sks"

    descripcion_programa = "%prog recompile STarKos song files."



    # definimos las opciones que soportaremos desde la lnea de comandos

    lista_de_opciones = [

        make_option("-a", "--address", action="store", type="string", dest="recompile_address", default="0x4000", help="Address to recompile in hex"),

#        make_option("-o", "--output", action="store", type="string", dest="fichero_salida", default="cancion.bin", help="Name for the song file")

        make_option("-v", "--verbose", action="store_true", dest="verbose", default=False, help="Print more info")

    ]

        

    parser = OptionParser(usage=uso_programa, description=descripcion_programa,

        version=version_programa, option_list=lista_de_opciones)

    

    # obtenemos las opciones y la lista de ficheros suministradas al programa

    (opciones, lista_ficheros_tmp) = parser.parse_args(linea_de_comandos)



    # comprobamos el número de argumentos y verificamos los valores

    if (not lista_ficheros_tmp):

        parser.error("No files to process.")

    else:

        lista_ficheros = []

        for i in lista_ficheros_tmp:

            lista_ficheros = lista_ficheros + glob.glob(i)

    try:

        opciones.recompile_address = int(opciones.recompile_address, 16)

        if (0 > opciones.recompile_address) or (opciones.recompile_address > 0x10000):

            raise Exception

    except:

        parser.error("Address to recompile invalid.")



    return opciones, lista_ficheros



# Función principal

def main(linea_de_comandos=None):

    """

    Función principal

    """

    # obtenemos las opciones y argumentos suministrados al programa

    opciones, lista_ficheros = procesar_linea_comandos(linea_de_comandos)



    for nombre_fichero in lista_ficheros:

        

        if not(os.path.exists(nombre_fichero)):

            print u"The file %s do not exist." % nombre_fichero

            continue



        # Abrimos el fichero            

        fichero_tmp = ""

        print u"Opening the file: %s" % nombre_fichero

        with open(nombre_fichero,"rb") as fichero:

            fichero_tmp = fichero.read()



        # Comprobamos si el fichero tenía una cabecera de Amsdos

        usa_cabecera_amsdos = fichero_tmp[128:132] == "SK10"

        if not ((fichero_tmp[0:4] == "SK10") or usa_cabecera_amsdos):

            print "The file %s is not a valid STarKos song." % nombre_fichero

            continue



        # Si usa cabecera de amsdos, extraemos la longitud y recortamos el fichero a dicho tamaño

        if usa_cabecera_amsdos:

            longitud_tmp = ord(fichero_tmp[65]) * 256 + ord(fichero_tmp[64])

            fichero_tmp = fichero_tmp[128: 128 + longitud_tmp]



        # Comprobamos que el fichero no haya sido ya recompilado

        original_address = ord(fichero_tmp[5]) * 256 + ord(fichero_tmp[4])

        if opciones.verbose:

            print "Original address: %s" % hex(original_address)

        print "Original address: %s" % hex(original_address)

        if original_address == opciones.recompile_address:

            print "The file %s already use the address pass how parameter." % nombre_fichero

            continue



        # Convertimos la cadena a una lista, para facilitarnos las modificaciones

        cancion_tmp = list(fichero_tmp)



        # Cambiamos la dirección de la canción en la cabecera

        cancion_tmp [4] = chr(opciones.recompile_address % 256)

        cancion_tmp [5] = chr(opciones.recompile_address // 256)



        # Obtenemos el offset entre ambas direcciones

        offset = opciones.recompile_address - original_address 

        if opciones.verbose:

            print "Offset: %s" % hex(offset)



        # Cambiamos el offset relativo de algunos punteros de uso interno

        if opciones.verbose:

            print "Change internal pointers."

        for i in range(9, 9 + 8 * 2, 2):

            if opciones.verbose:

                print hex(i),hex(ord(fichero_tmp[i]) + ord(fichero_tmp[i + 1]) * 256), 

                    hex(ord(fichero_tmp[i]) + ord(fichero_tmp[i + 1]) * 256 + offset)

            offset_tmp = ord(fichero_tmp[i]) + ord(fichero_tmp[i + 1]) * 256 + offset

            cancion_tmp[i] = chr(offset_tmp % 256)

            cancion_tmp[i + 1] = chr(offset_tmp // 256)



        # Obtenemos la localización de los Tracks, Stracks y los Instruments

        TRACKSTAB = ord(fichero_tmp[11]) + ord(fichero_tmp[12]) * 256 

        STRACKSTAB = ord(fichero_tmp[13]) + ord(fichero_tmp[14]) * 256 

        INSTRS = ord(fichero_tmp[15]) + ord(fichero_tmp[16]) * 256 

        if opciones.verbose:

            print "TRACKS: %s, STRACKS: %s, INSTRS: %s" % (hex(TRACKSTAB), hex(STRACKSTAB), hex(INSTRS))



        # Cambiamos la tabla de TRACKS

        origen_tmp = TRACKSTAB - original_address

        longitud_tmp = STRACKSTAB - TRACKSTAB

        print "TRACKS dir: %s off: %s len: %s" % (hex(TRACKSTAB), hex(origen_tmp), hex(longitud_tmp))

        for i in range(origen_tmp, origen_tmp + longitud_tmp, 2):

            if opciones.verbose:

                print hex(i), hex(ord(fichero_tmp[i]) + ord(fichero_tmp[i + 1]) * 256), 

                    hex(ord(fichero_tmp[i]) + ord(fichero_tmp[i + 1]) * 256 + offset)

            offset_tmp = ord(fichero_tmp[i]) + ord(fichero_tmp[i + 1]) * 256 + offset

            cancion_tmp[i] = chr(offset_tmp % 256)

            cancion_tmp[i + 1] = chr(offset_tmp // 256)



        # Cambiamos la tabla de STRACKS

        origen_tmp = STRACKSTAB - original_address

        longitud_tmp = INSTRS - STRACKSTAB

        print "STRACKS dir: %s off: %s len: %s" % (hex(STRACKSTAB), hex(origen_tmp), hex(longitud_tmp))

        contador_tmp = origen_tmp

        while (contador_tmp <> origen_tmp + longitud_tmp):

            if opciones.verbose:

                print "Contador:", hex(contador_tmp)

            num_elementos_tmp = ord(fichero_tmp[contador_tmp])

            num_elementos_tmp = (num_elementos_tmp >> 1 if num_elementos_tmp & 1 else 0)

            contador_tmp += 1            



            for j in range(num_elementos_tmp + 1):

                if opciones.verbose:

                    print "Contador:", hex(contador_tmp),

                offset_tmp = ord(fichero_tmp[contador_tmp]) + ord(fichero_tmp[contador_tmp + 1]) * 256 + offset

                if opciones.verbose:

                    print hex(offset_tmp)

                cancion_tmp[contador_tmp] = chr(offset_tmp % 256)

                cancion_tmp[contador_tmp + 1] = chr(offset_tmp // 256)

                contador_tmp += 2



        # Cambiamos la tabla de INSTRS

        origen_tmp = INSTRS - original_address

        longitud_tmp = ord(fichero_tmp[origen_tmp]) + ord(fichero_tmp[origen_tmp + 1]) * 256 - INSTRS

        print "INSTRS dir: %s off: %s len: %s" % (hex(INSTRS), hex(origen_tmp), hex(longitud_tmp))

        for i in range(origen_tmp, origen_tmp + longitud_tmp, 2):

            if opciones.verbose:

                print hex(i), hex(ord(fichero_tmp[i]) + ord(fichero_tmp[i + 1]) * 256), 

                    hex(ord(fichero_tmp[i]) + ord(fichero_tmp[i + 1]) * 256 + offset)

            offset_tmp = ord(fichero_tmp[i]) + ord(fichero_tmp[i + 1]) * 256 + offset

            cancion_tmp[i] = chr(offset_tmp % 256)

            cancion_tmp[i + 1] = chr(offset_tmp // 256)



        # Cambiamos los datos de los instrumentos

        contador_tmp = origen_tmp

        for i in range(origen_tmp, origen_tmp + longitud_tmp, 2):

            # Obtenemos el offset a los datos del instrumento

            offset_inst_tmp = ord(fichero_tmp[contador_tmp]) + ord(fichero_tmp[contador_tmp + 1]) * 256 - original_address

            contador_tmp += 2



            # Modificamos los dos punteros de los datos del instrumento

            for j in range(2):

                offset_tmp = ord(fichero_tmp[offset_inst_tmp]) + ord(fichero_tmp[offset_inst_tmp + 1]) * 256 + offset

                cancion_tmp[offset_inst_tmp] = chr(offset_tmp % 256)

                cancion_tmp[offset_inst_tmp + 1] = chr(offset_tmp // 256)

                if opciones.verbose:

                    print "off: %s dir: %s" % (hex(offset_inst_tmp), hex(offset_tmp))

                offset_inst_tmp += 2



        # Grabamos el fichero resultante

        nombre_fichero = nombre_fichero.lower().replace(".sks", ".bin") # + ".bin"

        print u"Saving the file: %s" % nombre_fichero

        with open(nombre_fichero,"wb") as fichero:

            fichero.write("".join(cancion_tmp))



    return 0    # EXIT_SUCCESS



if __name__ == "__main__":

    estado = main()

    sys.exit(estado)
