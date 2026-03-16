import { Routes, Route, Navigate } from 'react-router-dom'
import { useSelector } from 'react-redux'
import { AuthProvider } from './context/AuthProvider'
import Navbar from './components/Navbar'
import Login from './components/Login'
import Register from './components/Register'
import Dashboard from './components/Dashboard'
import Groups from './components/Groups'
import GroupDetails from './components/GroupDetails'

function ProtectedRoute({ children }) {
  const token = useSelector((state) => state.auth.token)
  return token ? children : <Navigate to="/login" replace />
}

function Layout({ children }) {
  return (
    <div className="min-h-screen bg-[#0e1012]">
      <Navbar />
      <main>{children}</main>
    </div>
  )
}

export default function App() {
  return (
    <AuthProvider>
      <Routes>
        <Route path="/login" element={<Login />} />
        <Route path="/register" element={<Register />} />
        <Route path="/dashboard" element={
          <ProtectedRoute><Layout><Dashboard /></Layout></ProtectedRoute>
        } />
        <Route path="/groups" element={
          <ProtectedRoute><Layout><Groups /></Layout></ProtectedRoute>
        } />
        <Route path="/groups/:id" element={
          <ProtectedRoute><Layout><GroupDetails /></Layout></ProtectedRoute>
        } />
        <Route path="*" element={<Navigate to="/dashboard" replace />} />
      </Routes>
    </AuthProvider>
  )
}