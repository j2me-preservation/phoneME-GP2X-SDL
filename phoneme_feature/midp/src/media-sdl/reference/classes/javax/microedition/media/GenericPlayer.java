/*
 * Copyright  1990-2007 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */
package javax.microedition.media;

import java.util.Vector;
import java.util.Enumeration;
import java.util.Hashtable;

import java.io.*;

import javax.microedition.media.*;
import javax.microedition.media.control.*;

public class GenericPlayer extends ABBBasicPlayer 
       { private String GenericPlayer_Type;

         public GenericPlayer(String playerType) 
                { super();
                  this.GenericPlayer_Type = playerType;
                }
    
         public int getAudioType() 
                { return AUDIO_PCM;
                }

         protected void doRealize() throws MediaException
                   { System.out.println("GenericPlayer["+GenericPlayer_Type+"]: doRealize()"); 
                   }

         protected void doPrefetch() throws MediaException
                   { System.out.println("GenericPlayer["+GenericPlayer_Type+"]: doPrefetch()");
                   }

         protected boolean doStart()
                   { System.out.println("GenericPlayer["+GenericPlayer_Type+"]: doStart()");
                     return true;
                   }
                   
         protected void doStop() throws MediaException
                   { System.out.println("GenericPlayer["+GenericPlayer_Type+"]: doStop()");
                   }

         protected void doDeallocate()
                   { System.out.println("GenericPlayer["+GenericPlayer_Type+"]: doDeallocate()");
                   }

         protected void doClose()
                   { System.out.println("GenericPlayer["+GenericPlayer_Type+"]: doClose()");
                   }

         protected long doSetMediaTime(long now) throws MediaException
                   { System.out.println("GenericPlayer["+GenericPlayer_Type+"]: doSetMediaTime("+String.valueOf(now)+")");
                     return now;
                   }

         protected long doGetMediaTime()
                   { System.out.println("GenericPlayer["+GenericPlayer_Type+"]: doGetMediaTime()");
                     return TIME_UNKNOWN;
                   }

         protected long doGetDuration()
                   { System.out.println("GenericPlayer["+GenericPlayer_Type+"]: doGetDuration()");
                     return TIME_UNKNOWN;
                   }

         protected Control doGetControl(String type)
                   { System.out.println("GenericPlayer["+GenericPlayer_Type+"]: doGetControl("+type+")");
                     return null;
                   }

         public String getContentType() 
                { chkClosed(true);
                  return this.GenericPlayer_Type;
                }

}


