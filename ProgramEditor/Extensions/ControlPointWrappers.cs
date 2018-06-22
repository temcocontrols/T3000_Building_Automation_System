using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProgramEditor.Extensions
{
    /// <summary>
    /// Enumerable Identifiers Types
    /// </summary>
    public enum IdentifierTypes
    {
        VARS, INS, OUTS, PRGS, SCHS, HOLS
    }

    /// <summary>
    /// Basic info for controlpoint identifiers
    /// </summary>
    public class ControlPointInfo
    {
        string _label = "";
        /// <summary>
        /// Label of controlpoint
        /// </summary>
        public string Label
        {
            get
            {
                return _label;
            }

            set
            {
                _label = value;
            }
        }

        IdentifierTypes _type = IdentifierTypes.VARS;
        //ControlPoint Type
        public IdentifierTypes Type
        {
            get
            {
                return _type;
            }

            set
            {
                _type = value;
            }
        }

        /// <summary>
        /// Default constructor
        /// </summary>
        public ControlPointInfo() { }

        /// <summary>
        /// Constructor with initialization
        /// </summary>
        /// <param name="LabelName">Label</param>
        /// <param name="Type">optional Type, default = VARS</param>
        public ControlPointInfo(string LabelName, IdentifierTypes Type = IdentifierTypes.VARS)
        {
            this.Label = LabelName;
            this.Type = Type;
        }

    }

    /// <summary>
    /// List of all enumerables identifiers/controlpoints
    /// </summary>
    public class ControlPoints
    {
        List<ControlPointInfo> _variables = new List<ControlPointInfo>();
        List<ControlPointInfo> _inputs = new List<ControlPointInfo>();
        List<ControlPointInfo> _outputs = new List<ControlPointInfo>();
        List<ControlPointInfo> _programs = new List<ControlPointInfo>();
        List<ControlPointInfo> _schedules = new List<ControlPointInfo>();
        List<ControlPointInfo> _holidays = new List<ControlPointInfo>();

        /// <summary>
        /// Variables identifiers
        /// </summary>
        public List<ControlPointInfo> Variables
        {
            get
            {
                return _variables;
            }

            set
            {
                _variables = value;
            }
        }
        /// <summary>
        /// Inputs identifiers
        /// </summary>
        public List<ControlPointInfo> Inputs
        {
            get
            {
                return _inputs;
            }

            set
            {
                _inputs = value;
            }
        }

        /// <summary>
        /// Outputs identifiers
        /// </summary>
        public List<ControlPointInfo> Outputs
        {
            get
            {
                return _outputs;
            }

            set
            {
                _outputs = value;
            }
        }

        /// <summary>
        /// Programs Identifiers
        /// </summary>
        public List<ControlPointInfo> Programs
        {
            get
            {
                return _programs;
            }

            set
            {
                _programs = value;
            }
        }

        /// <summary>
        /// Schedules identifiers
        /// </summary>
        public List<ControlPointInfo> Schedules
        {
            get
            {
                return _schedules;
            }

            set
            {
                _schedules = value;
            }
        }

        /// <summary>
        /// Holidays identifiers
        /// </summary>
        public List<ControlPointInfo> Holidays
        {
            get
            {
                return _holidays;
            }

            set
            {
                _holidays = value;
            }
        }

        /// <summary>
        /// Add new control points
        /// </summary>
        /// <param name="LabelName">Label</param>
        /// <param name="Type">Optional Type, defaults to VARS</param>
        public void Add(string LabelName, IdentifierTypes Type = IdentifierTypes.VARS)
        {

            ControlPointInfo newControlPoint = new ControlPointInfo(LabelName, Type);
            switch (Type)
            {
                case IdentifierTypes.VARS:
                    Variables.Add(newControlPoint);
                    break;
                case IdentifierTypes.INS:
                    Inputs.Add(newControlPoint);
                    break;
                case IdentifierTypes.OUTS:
                    Outputs.Add(newControlPoint);
                    break;
                case IdentifierTypes.PRGS:
                    Programs.Add(newControlPoint);
                    break;
                case IdentifierTypes.SCHS:
                    Schedules.Add(newControlPoint);
                    break;
                case IdentifierTypes.HOLS:
                    Holidays.Add(newControlPoint);
                    break;
                default:
                    break;
            }

        }

    }
}