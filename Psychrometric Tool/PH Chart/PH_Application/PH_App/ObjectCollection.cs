using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PH_App
{
   public class ObjectCollection 
    {
        public MainController mc { get; set; }
        public InitialDatabaseSetupController idsc { get; set; }
        public DatabaseOperationModel dom { get; set; }
        //public BuildingOperation bo { get; set; }
        //public ChartCreationAndOperations coprn { get; set; }
        
        public ObjectCollection()
        {
             mc = new MainController();
             idsc = new InitialDatabaseSetupController();
             dom = new DatabaseOperationModel();
           // bo = new BuildingOperation();
           //coprn = new ChartCreationAndOperations();
             
        }

}
}
