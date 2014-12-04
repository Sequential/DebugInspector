using System;
using System.Collections.Generic;
using System.Text;

namespace DebugInspector.Studio
{
    internal class TypeNames
    {
        private class TypeName
        {
            private readonly String r_ShorthandName;
            private readonly String r_ClrName;
            private readonly Type r_Type;

            TypeName(String shorthandName, Type type)
            {
                if(shorthandName == null)
                    throw new ArgumentNullException("cSharpName");

                if(type == null)
                    throw new ArgumentNullException("type");

                r_ShorthandName    = shorthandName;
                r_Type          = type;
                r_ClrName       = type.FullName;
            }

            public String ShortHandName { get { return r_ShorthandName; } }
            public String FullName { get { return r_ClrName; } }
            public Type ActualType { get { return r_Type; } }

            private static readonly Dictionary<String, TypeName> sr_CSharpMap = new Dictionary<string,TypeName>();
            private static readonly Dictionary<String, TypeName> sr_ClrMap    = new Dictionary<string,TypeName>();
            
            internal static void Add(String cSharpName, Type type)
            {
                TypeName typeName = new TypeName(cSharpName, type);
                sr_CSharpMap.Add(cSharpName, typeName);
                sr_ClrMap.Add(typeName.r_ClrName, typeName);
            }

            public static TypeName LookupFromShortHand(String shorthand)
            { 
                TypeName retVal = null;
                sr_CSharpMap.TryGetValue(shorthand, out  retVal);
                return retVal;
            }

            public static TypeName LookupFromTypeName(String typeName)
            {
                TypeName retVal = null;
                sr_ClrMap.TryGetValue(typeName, out  retVal);
                return retVal;
            }
        }

        static TypeNames()
        {
            TypeName.Add("sbyte",   typeof(sbyte)); 
            TypeName.Add("byte",    typeof(byte));
            TypeName.Add("short",   typeof(short));
            TypeName.Add("ushort",  typeof(ushort));
            TypeName.Add("int",     typeof(int));
            TypeName.Add("uint",    typeof(uint));
            TypeName.Add("long",    typeof(long));
            TypeName.Add("ulong",   typeof(ulong));
            TypeName.Add("float",   typeof(float));
            TypeName.Add("double",  typeof(double));
            TypeName.Add("bool",    typeof(bool));
            TypeName.Add("char",    typeof(char));
            TypeName.Add("string",  typeof(string));
            TypeName.Add("object",  typeof(object));
        }

        public static String GetFullName(String shorthand)
        {
            TypeName tn = TypeName.LookupFromShortHand(shorthand);
            if (tn == null)
                return null;

            return tn.FullName;
        }

        public static String GetShorthandName(String typeName)
        {
            TypeName tn = TypeName.LookupFromTypeName(typeName);
            if (tn == null)
                return null;
            return tn.ShortHandName;
        }

        public static String GetShorthandName(Type type)
        {
            TypeName tn = TypeName.LookupFromTypeName(type.FullName);
            if (tn == null)
                return null;
            return tn.ShortHandName;
        }
    }
}
